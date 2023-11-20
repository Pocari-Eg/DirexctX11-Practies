#include "GUIClass.h"



GUIClass::GUIClass()
{
 
}

GUIClass::GUIClass(const GUIClass&)
{
}

GUIClass::~GUIClass()
{
}

bool GUIClass::Initialize(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext,
    ID3D11RenderTargetView* RenderTargetView)
{

    m_device = device;
    m_deviceContext = deviceContext;
    g_mainRenderTargetView = RenderTargetView;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

    m_io = &io;
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();


    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(m_device, m_deviceContext);

    // Our state
     show_demo_window = false;
     show_another_window = false;
  
     return true;
}

void GUIClass::Shutdown()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void GUIClass::Draw()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");       
        // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
  

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / m_io->Framerate, m_io->Framerate);



        ImGui::End();
    }

    // 3. Show another simple window.

        //ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        //ImGui::Text("Hello from another window!");
        //if (ImGui::Button("Close Me"))
        //    show_another_window = false;
        //ImGui::End();

        bool open = false, save = false;
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("Menu"))
            {
                if (ImGui::MenuItem("Open", NULL))
                    open = true;
                if (ImGui::MenuItem("Save", NULL))
                    save = true;

                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        //Remember the name to ImGui::OpenPopup() and showFileDialog() must be same...
        if (open)
            ImGui::OpenPopup("Open File");
        if (save)
            ImGui::OpenPopup("Save File");
        
 
        /* Optional third parameter. Support opening only compressed rar/zip files.
         * Opening any other file will show error, return false and won't close the dialog.
         */
        if (file_dialog.showFileDialog("Open File", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(700, 310),".fbx"))
        {
            std::cout << file_dialog.selected_fn << std::endl;      // The name of the selected file or directory in case of Select Directory dialog mode
            std::cout << file_dialog.selected_path << std::endl;    // The absolute path to the selected file
        }
        if (file_dialog.showFileDialog("Save File", imgui_addons::ImGuiFileBrowser::DialogMode::SAVE, ImVec2(700, 310), ".png,.jpg,.bmp"))
        {
            std::cout << file_dialog.selected_fn << std::endl;      // The name of the selected file or directory in case of Select Directory dialog mode
            std::cout << file_dialog.selected_path << std::endl;    // The absolute path to the selected file
            std::cout << file_dialog.ext << std::endl;              // Access ext separately (For SAVE mode)
            //Do writing of files based on extension here
        }


    // Rendering
    ImGui::Render();

    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
