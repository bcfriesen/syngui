#include <iostream>
#include <gtkmm.h>
#include <yaml-cpp/yaml.h>

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

    /* Eventuall this YAML emitter will be much more automagic. */
    YAML::Node node;

    node["output"]["min_wl"] =  3300.0;
    node["output"]["max_wl"] = 10000.0;
    node["output"]["wl_step"] =    5.0;

    node["grid"]["bin_width"] =    0.3;
    node["grid"]["v_size"] =     100;
    node["grid"]["v_outer_max"] = 30.0;

    node["opacity"]["line_dir"]    = "/Users/brian/es-data/lines";
    node["opacity"]["ref_File"]    = "/Users/brian/es-data/refs.dat";
    node["opacity"]["form"]        = "hexp";
    node["opacity"]["v_ref"]       = 6.0;
    node["opacity"]["log_tau_min"] = -2.0;

    node["source"]["mu_size"] = 10;

    node["spectrum"]["p_size"] = 60;
    node["spectrum"]["flatten"] = "No";

    node["setups"]["a0"] = 1.0;
    node["setups"]["a1"] = 0.0;
    node["setups"]["a2"] = 0.0;
    node["setups"]["v_phot"] = 10.0;
    node["setups"]["v_outer"] = 60.0;
    node["setups"]["t_phot"] = 12.0;
    node["setups"]["t_phot"] = 12.0;

    node["setups"]["ions"].push_back(kid1);
    node["setups"]["active"].push_back("Yes");
    node["setups"]["log_tau"].push_back(0.3);
    node["setups"]["v_min"].push_back(6.0);
    node["setups"]["v_max"].push_back(30.0);
    node["setups"]["aux"].push_back(1.0);
    node["setups"]["temp"].push_back(7.0);

    node["setups"]["ions"].push_back(kid2);
    node["setups"]["active"].push_back("No");
    node["setups"]["log_tau"].push_back(0.5);
    node["setups"]["v_min"].push_back(6.5);
    node["setups"]["v_max"].push_back(30.5);
    node["setups"]["aux"].push_back(1.5);
    node["setups"]["temp"].push_back(7.5);

    YAML::Emitter out;
    out << node;

    std::cout << out.c_str() << std::endl;
    std::cout << std::endl << std::endl;
}

int main(int argc, char* argv[])
{
 /* make sure to call this FIRST, otherwise you'll get spammed with errors about
  * 'you forgot to call g_type_init()!' */
    Gtk::Main kit(argc, argv);

    Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create();

    try {
        builder->add_from_file("syn++-gui-gtk2.glade");
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
