#include <wx/image.h>
#include <wx/statbmp.h>
#include <wx/wx.h>

//
#include <Windows.h>

class MyFrame : public wxFrame {
public:
    MyFrame()
        : wxFrame(nullptr, wxID_ANY, "wxWidgets Example", wxDefaultPosition, wxSize(300, 400)) {
        SetBackgroundColour(*wxBLACK);

        wxMenu* helpMenu = new wxMenu;
        helpMenu->Append(wxID_ABOUT, "&About\tF1", "Show about dialog");

        wxMenuBar* menuBar = new wxMenuBar;
        menuBar->Append(helpMenu, "&Help");
        SetMenuBar(menuBar);

        Bind(wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);

        wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

        wxImage::AddHandler(new wxPNGHandler);
        wxImage         image("Logo.png", wxBITMAP_TYPE_PNG);
        wxStaticBitmap* bitmap = new wxStaticBitmap(this, wxID_ANY, wxBitmap(image));
        sizer->Add(bitmap, 0, wxALL | wxALIGN_CENTER, 10);

        checkList = new wxCheckListBox(this, wxID_ANY);
        checkList->Append("a text");
        checkList->Append("b text");
        checkList->Append("c text");
        sizer->Add(checkList, 0, wxALL | wxEXPAND, 10);

        wxButton* goButton = new wxButton(this, wxID_ANY, "Go!");
        sizer->Add(goButton, 0, wxALL | wxALIGN_CENTER, 10);

        Bind(wxEVT_BUTTON, &MyFrame::OnGo, this, goButton->GetId());

        SetSizerAndFit(sizer);
    }

private:
    void OnAbout(wxCommandEvent& WXUNUSED(event)) {
        wxMessageBox(
            "This is a wxWidgets example program", "About", wxOK | wxICON_INFORMATION, this
        );
    }
    void OnGo(wxCommandEvent& WXUNUSED(event)) {
        wxString selectedItems;
        for (size_t i = 0; i < checkList->GetCount(); ++i) {
            if (checkList->IsChecked(i)) {
                selectedItems += checkList->GetString(i) + "\n";
            }
        }
        if (!selectedItems.IsEmpty()) {
            wxMessageBox(selectedItems, "Selected Items", wxOK | wxICON_INFORMATION, this);
        } else {
            wxMessageBox("No items selected.", "Selected Items", wxOK | wxICON_WARNING, this);
        }
    }
    wxCheckListBox* checkList;
};

class MyApp : public wxApp {
public:
    bool OnInit() {
        MyFrame* frame = new MyFrame();
        frame->Show(true);
        return true;
    }
};

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    wxMessageBox("We did something!");

    auto* app = new MyApp;
    wxApp::SetInstance(app);

    int    argc = 0;
    char** argv = nullptr;
    if (wxEntryStart(argc, argv)) {
        if (wxTheApp->OnInit()) wxTheApp->OnRun();
        wxTheApp->OnExit();
        wxEntryCleanup();
    }

    return 0;
}