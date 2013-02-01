#include <iostream>
#include <gtkmm.h>

/* pointers for all the widgets */
Gtk::Window *pWindow = 0;
Gtk::Button *pQuitButton = 0, *pSaveParamsButton = 0;
Gtk::SpinButton *pKIDSpin1 = 0, *pKIDSpin2 = 0;

unsigned int kid1, kid2; /* Kurucz IDs for each species */

static void
quit_syngui()
{
    if (pWindow) {
        pWindow->hide();
    }
}

static void
print_kids()
{
    kid1 = pKIDSpin1->get_value();
    kid2 = pKIDSpin2->get_value();
    std::cout << "Kurucz ID #1: " << kid1 << std::endl;
    std::cout << "Kurucz ID #2: " << kid2 << std::endl;
}

int main(int argc, char* argv[])
{
 /* make sure to call this FIRST, otherwise you'll get spammed with errors about
  * 'you forgot to call g_type_init()!' */
    Gtk::Main kit(argc, argv);

    Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create();

    try {
        builder->add_from_file("../syn++-gui-gtk2.glade");
    } catch(const Glib::FileError& ex) {
        std::cerr << "FileError: " << ex.what() << std::endl;
        return 1;
    } catch(const Glib::MarkupError& ex) {
        std::cerr << "MarkupError: " << ex.what() << std::endl;
        return 1;
    } catch(const Gtk::BuilderError& ex) {
        std::cerr << "BuilderError: " << ex.what() << std::endl;
        return 1;
    }

    builder->get_widget("syn++-gui_window", pWindow);

    builder->get_widget("quit_button", pQuitButton);
    if (pQuitButton) {
        pQuitButton->signal_clicked().connect (sigc::ptr_fun(quit_syngui));
    }

    builder->get_widget("save_params_button", pSaveParamsButton);
    if (pSaveParamsButton) {
        pSaveParamsButton->signal_clicked().connect (sigc::ptr_fun(print_kids));
    }

    builder->get_widget("s01_kurucz_id_spinbtn", pKIDSpin1);
    builder->get_widget("s02_kurucz_id_spinbtn", pKIDSpin2);

    kit.run(*pWindow);

    delete pWindow;
    delete pQuitButton;
    delete pSaveParamsButton;
    delete pKIDSpin1;
    delete pKIDSpin2;

    return 0;
}
