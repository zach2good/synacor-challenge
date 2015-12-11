#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stack>
#include <algorithm>

#include <SDL/SDL.h>

#include "Window.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

#include "console.h"

#include "vm.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

typedef unsigned short ushort;
typedef unsigned char uchar;

using namespace std;

// SDL Window 
auto window = Window("Synacor VM", 1280, 720);

// VM
VM vm = VM();

bool im_openMemoryEditor = true; 
int im_memoryEditorPage = 0;
bool im_memoryFollowPointer = true;
bool im_openRegisterEditor = true; 
bool im_openStackEditor = true; 
bool im_openConsole = true;

bool run_vm = false;

static char input_buffer[64];

string digestPreview;
 
void main_loop()
{
    // Handle Input
    SDL_Event event;  
    while (SDL_PollEvent(&event))
    {
    	ImGui_ImplSdl_ProcessEvent(&event);
    }

    // Update
    if (vm.isRunning)
    {
        vm.step();
    }

    // Prepare GUI
    ImGui_ImplSdl_NewFrame(window.getWindow());
    static Console console;

    if (im_openConsole)
    {
        console.Draw("Console", &im_openConsole);
        console.UpdateDigestBuffer(vm.digest);

        if (vm.stringDigest)
        {
            console.AddLog(vm.digest.c_str());
            digestPreview = vm.digest;
            vm.digest = "";
            vm.stringDigest = false;
        }
    }   

    ImGui::SetNextWindowSize(ImVec2(250,400), ImGuiSetCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(30, 30), ImGuiSetCond_FirstUseEver);

    ImGui::Begin("Synacor VM");

        if (vm.isRunning)
        {
           ImGui::Text("Status: VM Running"); 
        }
        else
        {
            ImGui::Text("Status: VM Halted"); 
        }
            
        ImGui::Checkbox("Debug", vm.getDebug());
        ImGui::Text("Instruction Count: %d", vm.getInstructionCount()); 
        ImGui::Text("Memory Ptr: %d", vm.getMemoryPtr()); 
        ImGui::Text("Self-Test ends at 701400 instructions"); 
        
        if (ImGui::Button("Step"))
        {
            vm.step();
        } 

        if (!vm.isRunning)
        {
           if (ImGui::Button("Run VM")) vm.isRunning = true; 

        }   
        else
        {
            if (ImGui::Button("Pause VM")) vm.isRunning = false;
        }
        ImGui::Text(""); 

        if (ImGui::Button("Console")) im_openConsole = !im_openConsole;
        if (ImGui::Button("Memory Editor")) im_openMemoryEditor = !im_openMemoryEditor;
        if (ImGui::Button("Stack Editor")) im_openStackEditor = !im_openStackEditor;
        if (ImGui::Button("Register Editor")) im_openRegisterEditor = !im_openRegisterEditor;

        ImGui::Text(""); 

        if (ImGui::Button("Restore State (dump_695700.bin)")) vm.restoreState("dump_695700.bin");
        
        if (ImGui::Button("Help"))
        {
            console.AddLog("0> 21 OP_NOOP\n");
            console.AddLog("1> 21 OP_NOOP\n");
            console.AddLog("2> 19 OP_OUT W\n");
            console.AddLog("4> 19 OP_OUT e\n");
            console.AddLog("6> 19 OP_OUT l\n");
            console.AddLog("8> 19 OP_OUT c\n");
            console.AddLog("10> 19 OP_OUT o\n");
            console.AddLog("12> 19 OP_OUT m\n");
            console.AddLog("14> 19 OP_OUT e\n");
        } 
        
    ImGui::End();

    if (im_openRegisterEditor)
    {
        ImGui::SetNextWindowSize(ImVec2(250,255), ImGuiSetCond_FirstUseEver);
        ImGui::SetNextWindowPos(ImVec2(30,430), ImGuiSetCond_FirstUseEver);

        ImGui::Begin("Register Editor");
            ImGui::Columns(2, "Registers");
            ImGui::Text("Address"); ImGui::NextColumn();
            ImGui::Text("Value"); ImGui::NextColumn();
            ImGui::Separator();
            ImGui::Separator();

            for (int i = 0; i < 8; ++i)
            {
                ImGui::Text("R%d", i); ImGui::NextColumn();
                ImGui::Text("%d", vm.getRegisters()[i]); ImGui::NextColumn();
                ImGui::Separator();
            }
        ImGui::End();
    }

    if (im_openStackEditor)
    {
        ImGui::SetNextWindowSize(ImVec2(200,400), ImGuiSetCond_FirstUseEver);
        ImGui::SetNextWindowPos(ImVec2(280,30), ImGuiSetCond_FirstUseEver);

        auto stack = vm.getStack(); 
        vector<ushort> v = vector<ushort>();
        while(!stack.empty())
        {
            v.push_back(stack.top());
            stack.pop();
        }


        ImGui::Begin("Stack Editor");
            ImGui::Columns(2, "Stack");
            ImGui::Text("Depth"); ImGui::NextColumn();
            ImGui::Text("Value"); ImGui::NextColumn();
            ImGui::Separator();
            ImGui::Separator();

            for (int i = 0; i < v.size(); ++i)
            {
                ImGui::Text("%d", i); ImGui::NextColumn();
                ImGui::Text("%d", v.at(i)); ImGui::NextColumn();
                ImGui::Separator();
            }

        ImGui::End();
    }

    if (im_openMemoryEditor)
    {
        ImGui::SetNextWindowSize(ImVec2(350,400), ImGuiSetCond_FirstUseEver);
        ImGui::SetNextWindowPos(ImVec2(480,30), ImGuiSetCond_FirstUseEver);

        auto mem = vm.getMemory();
        auto memPtr = vm.getMemoryPtr();

        ImGui::Begin("Memory Editor");
        
        ImGui::Columns(2, "Memory");

        ImGui::Text("Memory Ptr: %d", memPtr); ImGui::NextColumn();
        ImGui::Checkbox("Follow MemPtr", &im_memoryFollowPointer); ImGui::NextColumn();

        ImGui::Separator();
        ImGui::Separator();

        if (im_memoryFollowPointer)
        {
            im_memoryEditorPage = memPtr / 15;
        }
        else
        {
            if (ImGui::Button("Prev")) if (im_memoryEditorPage != 0) im_memoryEditorPage--; ImGui::NextColumn();
            if (ImGui::Button("Next")) im_memoryEditorPage++; ImGui::NextColumn();

             ImGui::Separator();
            ImGui::Separator();  
        }    

        ImGui::Text("Memory pos"); ImGui::NextColumn();
        ImGui::Text("Value"); ImGui::NextColumn();
        ImGui::Separator();
        ImGui::Separator();

        int im_memStart = im_memoryEditorPage * 15;
        int im_memEnd = im_memStart + 15;
        for (int i = im_memStart; i < im_memEnd; ++i)
        {

            if (i == memPtr) 
            {
                ImGui::TextColored(ImColor(255,0,0), "%d", i); ImGui::NextColumn();
                ImGui::TextColored(ImColor(255,0,0), "%d", mem->at(i)); ImGui::NextColumn();
            }
            else
            {
                ImGui::Text("%d", i); ImGui::NextColumn();
                ImGui::Text("%d", mem->at(i)); ImGui::NextColumn();
            }

            ImGui::Separator();
        }
            
        ImGui::End();
    } 

    // Render
    window.clear();
    ImGui::Render();
    window.swap();
}

int main() {
	
	ImGui_ImplSdl_Init(window.getWindow());

 	emscripten_set_main_loop(main_loop, 10000, 1);

  	ImGui_ImplSdl_Shutdown();

	return 0;
}

