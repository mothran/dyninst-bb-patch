#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string>
#include <vector>
#include "BPatch.h"
#include "BPatch_binaryEdit.h"
#include "BPatch_flowGraph.h"
#include "BPatch_function.h"
#include "BPatch_point.h"

#include "BPatch_object.h"
#include "BPatch_module.h"

using namespace Dyninst;

int main(int argc, char *argv[])
{
    if (argc != 3) {
        std::cerr << "Invalid args, ./patcher /path/to/input_bin /path/to/output_bin" << std::endl;
        return 1;
    }

    auto input_path = std::string(argv[1]);
    auto output_path = std::string(argv[2]);
    std::string runtime = "libruntime.so";

    BPatch bpatch;
    BPatch_binaryEdit *appBin = bpatch.openBinary(input_path.c_str(), false);

    if (!appBin->loadLibrary(runtime.c_str())) {
        std::cerr << "Failed to load runtime: " << runtime << std::endl;
        return 1;
    }
    BPatch_image *appImg = appBin->getImage();

    std::vector<BPatch_function *> funcs;
    appImg->findFunction("bb_callback", funcs);
    if (funcs.size() != 1) {
        std::cerr << "Failed to resolve bb_callback in runtime lib: " << runtime << std::endl;
        return 1;
    }
    
    BPatch_function *instBBCallback = funcs[0];

    std::vector<BPatch_module *> mods;
    if (!appImg->getModules(mods)) {
        std::cerr << "Failed to get module list for: " << input_path << std::endl;
        return 1;
    }

    for (BPatch_module *mod : mods)
    {
        char buf[4096];
        memset(buf, 0, sizeof(buf));
        mod->getName(buf, sizeof(buf) - 1);
        std::string mod_name = buf;

        if (mod_name == runtime) {
            continue;
        }
        std::cout << "Module: " << mod_name << std::endl;

        std::vector<BPatch_function *> funcs;
        if (!mod->getProcedures(funcs)) {
            std::cerr << "Failed to get functions for module: " << mod_name << std::endl;
            return 1;
        }

        for (BPatch_function *func : funcs) {
            std::string func_name = func->getName();

            if(func_name == "_start") {
                continue;
            }

            std::cout << "func: " << func_name << std::endl;
            BPatch_flowGraph *CFG = func->getCFG();
            if (!CFG) {
                continue;
            }

            BPatch_Set<BPatch_basicBlock *> bbs;
            if (!CFG->getAllBasicBlocks(bbs)) {
                std::cerr << "Failed to get basicblocks for: " << func_name << " in: " << mod_name << std::endl;
                continue;
            }

            for(BPatch_basicBlock *bb : bbs) {
                uint32_t start_addr = (uint32_t) bb->getStartAddress();

                BPatch_point *bbEntry = bb->findEntryPoint();
                if (!bbEntry) {
                    continue;
                }

                std::vector<BPatch_snippet *> instArgs;
                BPatch_constExpr exprCompID(start_addr);
                instArgs.push_back(&exprCompID);
                BPatch_funcCallExpr instBBExpr(*instBBCallback, instArgs);

                BPatchSnippetHandle *handle = appBin->insertSnippet(instBBExpr,
                                                                    *bbEntry,
                                                                    BPatch_callBefore,
                                                                    BPatch_lastSnippet);
                if(!handle) {
                    std::cerr << "Failed to instrument " << func_name << " in mod: " << mod_name << " at address 0x" << std::hex << start_addr << std::endl;
                }
            }
        }
    }

    appBin->writeFile(output_path.c_str());
    return 0;
}