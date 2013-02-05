#include <iostream>
#include <gtkmm.h>
#include <yaml-cpp/yaml.h>

/* pointers for all the widgets */
Gtk::Window *pWindow = 0;
Gtk::Button *pQuitButton = 0, *pSaveParamsButton = 0;
Gtk::SpinButton *pv_phot_spinbtn = 0, *pT_phot_spinbtn = 0, *pv_outer_spinbtn = 0,
                *pmin_wl_spinbtn = 0, *pmax_wl_spinbtn = 0, *pwl_step_spinbtn = 0;
Gtk::SpinButton *p_s01_kid_spinbtn    = 0, *p_s02_kid_spinbtn    = 0,
                *p_s01_logtau_spinbtn = 0, *p_s02_logtau_spinbtn = 0,
                *p_s01_v_min_spinbtn  = 0, *p_s02_v_min_spinbtn  = 0,
                *p_s01_v_max_spinbtn  = 0, *p_s02_v_max_spinbtn  = 0,
                *p_s01_aux_spinbtn    = 0, *p_s02_aux_spinbtn    = 0,
                *p_s01_T_ex_spinbtn   = 0, *p_s02_T_ex_spinbtn   = 0;
Gtk::CheckButton *p_s01_active_checkbtn = 0, *p_s02_active_checkbtn = 0;

static void quit_syngui() {
    if (pWindow) {
        pWindow->hide();
    }
}

static void print_kids() {
    /* Eventually this YAML emitter will be much more automagic. */
    YAML::Node node;

    // first set the setup-independent parameters
    node["output"]["min_wl"] =  pmin_wl_spinbtn->get_value();
    node["output"]["max_wl"] = pmax_wl_spinbtn->get_value();
    node["output"]["wl_step"] = pwl_step_spinbtn->get_value();

    node["grid"]["bin_width"] =    0.3;
    node["grid"]["v_size"] =     100;
    node["grid"]["v_outer_max"] = 60.0;

    node["opacity"]["line_dir"]    = "/Users/brian/es-data/lines";
    node["opacity"]["ref_file"]    = "/Users/brian/es-data/refs.dat";
    node["opacity"]["form"]        = "exp";
    node["opacity"]["v_ref"]       = 6.0;
    node["opacity"]["log_tau_min"] = -2.0;

    node["source"]["mu_size"] = 10;

    node["spectrum"]["p_size"] = 60;
    node["spectrum"]["flatten"] = "No";

    // save each setup as a separate node
    YAML::Node setup1;

    setup1["a0"] = 1.0;
    setup1["a1"] = 0.0;
    setup1["a2"] = 0.0;

    setup1["v_phot"] = 10.0;
    setup1["v_outer"] = 60.0;
    setup1["t_phot"] = 12.0;

    std::vector<int> ions;
    ions.push_back(p_s01_kid_spinbtn->get_value());
    ions.push_back(p_s02_kid_spinbtn->get_value());
    setup1["ions"] = ions;

    std::vector<std::string> active;
    active.push_back(p_s01_active_checkbtn->get_active() ? "Yes" : "No");
    active.push_back(p_s02_active_checkbtn->get_active() ? "Yes" : "No");
    setup1["active"] = active;

    std::vector<double> log_tau;
    log_tau.push_back(p_s01_logtau_spinbtn->get_value());
    log_tau.push_back(p_s02_logtau_spinbtn->get_value());
    setup1["log_tau"] = log_tau;

    std::vector<double> v_min;
    v_min.push_back(p_s01_v_min_spinbtn->get_value());
    v_min.push_back(p_s02_v_min_spinbtn->get_value());
    setup1["v_min"] = v_min;

    std::vector<double> v_max;
    v_max.push_back(p_s01_v_max_spinbtn->get_value());
    v_max.push_back(p_s02_v_max_spinbtn->get_value());
    setup1["v_max"] = v_max;

    std::vector<double> aux;
    aux.push_back(p_s01_aux_spinbtn->get_value());
    aux.push_back(p_s02_aux_spinbtn->get_value());
    setup1["aux"] = aux;

    std::vector<double> temp;
    temp.push_back(p_s01_T_ex_spinbtn->get_value());
    temp.push_back(p_s02_T_ex_spinbtn->get_value());
    setup1["temp"] = temp;

    YAML::Emitter out;

    out << node; // first dump setup-indepenent parameters

    // now dump each setup
    out << YAML::BeginMap;
    out << YAML::Key << "setups";

    out << YAML::Value;
    out << YAML::BeginSeq;
    out << setup1; // if you had more than one setup you would dump the rest here
    out << YAML::EndSeq;

    out << YAML::EndMap;

    std::cout << out.c_str() << std::endl;
}

int main(int argc, char* argv[]) {
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

    builder->get_widget("quit_btn", pQuitButton);
    if (pQuitButton) {
        pQuitButton->signal_clicked().connect (sigc::ptr_fun(quit_syngui));
    }

    builder->get_widget("save_params_btn", pSaveParamsButton);
    if (pSaveParamsButton) {
        pSaveParamsButton->signal_clicked().connect (sigc::ptr_fun(print_kids));
    }

    builder->get_widget("s01_kid_spinbtn", p_s01_kid_spinbtn);
    builder->get_widget("s01_active_checkbtn", p_s01_active_checkbtn);
    builder->get_widget("s01_logtau_spinbtn", p_s01_logtau_spinbtn);
    builder->get_widget("s01_v_min_spinbtn", p_s01_v_min_spinbtn);
    builder->get_widget("s01_v_max_spinbtn", p_s01_v_max_spinbtn);
    builder->get_widget("s01_aux_spinbtn", p_s01_aux_spinbtn);
    builder->get_widget("s01_T_ex_spinbtn", p_s01_T_ex_spinbtn);

    builder->get_widget("s02_kid_spinbtn", p_s02_kid_spinbtn);
    builder->get_widget("s02_active_checkbtn", p_s02_active_checkbtn);
    builder->get_widget("s02_logtau_spinbtn", p_s02_logtau_spinbtn);
    builder->get_widget("s02_v_min_spinbtn", p_s02_v_min_spinbtn);
    builder->get_widget("s02_v_max_spinbtn", p_s02_v_max_spinbtn);
    builder->get_widget("s02_aux_spinbtn", p_s02_aux_spinbtn);
    builder->get_widget("s02_T_ex_spinbtn", p_s02_T_ex_spinbtn);

    builder->get_widget("v_phot_spinbtn", pv_phot_spinbtn);
    builder->get_widget("T_phot_spinbtn", pT_phot_spinbtn);
    builder->get_widget("v_outer_spinbtn", pv_outer_spinbtn);
    builder->get_widget("min_wl_spinbtn", pmin_wl_spinbtn);
    builder->get_widget("max_wl_spinbtn", pmax_wl_spinbtn);
    builder->get_widget("wl_step_spinbtn", pwl_step_spinbtn);

    kit.run(*pWindow);

    delete pWindow;
    delete pQuitButton;
    delete pSaveParamsButton;
    delete pv_phot_spinbtn;
    delete pT_phot_spinbtn;
    delete pv_outer_spinbtn;
    delete pmin_wl_spinbtn;
    delete pmax_wl_spinbtn;
    delete pwl_step_spinbtn;

    delete p_s01_kid_spinbtn;
    delete p_s01_active_checkbtn;
    delete p_s01_logtau_spinbtn;
    delete p_s01_v_min_spinbtn;
    delete p_s01_v_max_spinbtn;
    delete p_s01_aux_spinbtn;
    delete p_s01_T_ex_spinbtn;

    delete p_s02_kid_spinbtn;
    delete p_s02_active_checkbtn;
    delete p_s02_logtau_spinbtn;
    delete p_s02_v_min_spinbtn;
    delete p_s02_v_max_spinbtn;
    delete p_s02_aux_spinbtn;
    delete p_s02_T_ex_spinbtn;

    return 0;
}
