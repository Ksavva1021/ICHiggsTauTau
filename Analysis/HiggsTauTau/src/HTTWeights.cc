#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTWeights.h"
#include "UserCode/ICHiggsTauTau/interface/TriggerPath.hh"
#include "UserCode/ICHiggsTauTau/interface/TriggerObject.hh"
#include "UserCode/ICHiggsTauTau/interface/Electron.hh"
#include "UserCode/ICHiggsTauTau/interface/Muon.hh"
#include "UserCode/ICHiggsTauTau/interface/Tau.hh"
#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/BTagWeight.h"
#include "TMath.h"
#include "TSystem.h"
#include "TFile.h"
#include "boost/format.hpp"

namespace ic {

  HTTWeights::HTTWeights(std::string const& name) : ModuleBase(name),
    channel_(channel::et),
    mc_(mc::summer12_53X),
    era_(era::data_2012_rereco),
    strategy_(strategy::paper2013){
    do_trg_weights_             = false;
    trg_applied_in_mc_          = false;
    do_single_lepton_trg_       = false;
    do_cross_trg_               = false;
    do_singlemu_trg_weights_    = false;
    do_etau_fakerate_           = false;
    do_mtau_fakerate_           = false;
    do_jlepton_fake_            = false;
    do_idiso_weights_           = false;
    do_id_weights_              = false;
    do_emu_e_fakerates_         = false;
    do_emu_m_fakerates_         = false;
    do_top_factors_             = false;
    do_btag_weight_             = false;
    do_zpt_weight_              = false;
    do_tracking_eff_            = false;
    btag_mode_                  = 0;
    bfake_mode_                 = 0;
    tt_trg_iso_mode_            = 0;
    ggh_mass_                   = "";
    ggh_hist_                   = nullptr;
    ggh_hist_up_                = nullptr;
    ggh_hist_down_              = nullptr;
    do_tau_mode_scale_          = false;
    do_topquark_weights_        = false;
    do_top_jeteta_weights_      = false;
    do_tau_fake_weights_        = false;
    do_tt_muon_weights_         = false;
    do_em_qcd_weights_          = false;
    gen_tau_collection_         = "genParticlesTaus";
    jets_label_                 = "pfJetsPFlow";
    btag_label_                 = "combinedSecondaryVertexBJetTags";
    ditau_label_                = "emtauCandidates";
    z_pt_mass_hist_             = nullptr;
    mt_idiso_mc_                = nullptr;
    mt_idiso_data_              = nullptr;
    et_idiso_mc_                = nullptr;
    et_idiso_data_              = nullptr;
    em_m_idiso_mc_              = nullptr;
    em_m_idiso_data_            = nullptr;
    em_e_idiso_mc_              = nullptr;
    em_e_idiso_data_            = nullptr;
    em_m17_trig_mc_             = nullptr;
    em_m17_trig_data_           = nullptr;
    em_m8_trig_mc_              = nullptr;
    em_m8_trig_data_            = nullptr;
    em_e17_trig_mc_             = nullptr;
    em_e17_trig_data_           = nullptr;
    em_e12_trig_mc_             = nullptr;
    em_e12_trig_data_           = nullptr;
    et_trig_mc_                 = nullptr;
    et_trig_data_               = nullptr;
    et_antiiso1_trig_data_      = nullptr;
    et_antiiso2_trig_data_      = nullptr;
    et_xtrig_mc_                = nullptr;
    et_xtrig_data_              = nullptr;
    et_conditional_data_        = nullptr;
    et_conditional_mc_          = nullptr;
    mt_trig_mc_                 = nullptr;
    mt_trig_data_               = nullptr;
    mt_antiiso1_trig_data_      = nullptr;
    mt_antiiso2_trig_data_      = nullptr;
    mt_xtrig_mc_                = nullptr;
    mt_xtrig_data_              = nullptr;
    mt_conditional_data_        = nullptr;
    mt_conditional_mc_          = nullptr;
    em_qcd_cr1_lt2_             = nullptr;
    em_qcd_cr1_2to4_            = nullptr;
    em_qcd_cr1_gt4_             = nullptr;
    em_qcd_cr2_lt2_             = nullptr;
    em_qcd_cr2_2to4_            = nullptr;
    em_qcd_cr2_gt4_             = nullptr;
    ele_tracking_sf_            = nullptr;
    muon_tracking_sf_           = nullptr;
    scalefactor_file_           = "";
    scalefactor_file_ggh_       = "";
    do_tau_id_sf_               = false;
    mssm_higgspt_file_          = "";
    do_mssm_higgspt_            = false;
    do_z_weights_               = false;
    embedding_scalefactor_file_ = "";
    is_embedded_                = false;
    do_quarkmass_higgspt_       = false;
    do_ps_weights_              = false;
    do_nnlops_weights_          = false;
  }
  HTTWeights::~HTTWeights() {
    ;
  }

  int HTTWeights::PreAnalysis() {
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "HTTWeights" << std::endl;
    std::cout << "-------------------------------------" << std::endl;
    std::cout << boost::format(param_fmt()) % "channel"             % Channel2String(channel_);
    std::cout << boost::format(param_fmt()) % "era"                 % Era2String(era_);
    std::cout << boost::format(param_fmt()) % "mc"                  % MC2String(mc_);
    std::cout << boost::format(param_fmt()) % "strategy"        % Strategy2String(strategy_);
    std::cout << boost::format(param_fmt()) % "do_trg_weights"      % do_trg_weights_;
    if (do_trg_weights_) {
      std::cout << boost::format(param_fmt()) % "trg_applied_in_mc" % trg_applied_in_mc_;
    }
    std::cout << boost::format(param_fmt()) % "do_idiso_weights"    % do_idiso_weights_;
    std::cout << boost::format(param_fmt()) % "do_id_weights"       % do_id_weights_;
    std::cout << boost::format(param_fmt()) % "do_tau_mode_scale"   % do_tau_mode_scale_;
    std::cout << boost::format(param_fmt()) % "do_etau_fakerate"    % do_etau_fakerate_;
    std::cout << boost::format(param_fmt()) % "do_mtau_fakerate"    % do_mtau_fakerate_;
    std::cout << boost::format(param_fmt()) % "do_jlepton_fake"    % do_jlepton_fake_;
    std::cout << boost::format(param_fmt()) % "do_emu_e_fakerates"  % do_emu_e_fakerates_;
    std::cout << boost::format(param_fmt()) % "do_emu_m_fakerates"  % do_emu_m_fakerates_;
    std::cout << boost::format(param_fmt()) % "do_top_factors"      % do_top_factors_;
    std::cout << boost::format(param_fmt()) % "do_btag_weight"      % do_btag_weight_;
    std::cout << boost::format(param_fmt()) % "do_zpt_weight"       % do_zpt_weight_;
    std::cout << boost::format(param_fmt()) % "btag_mode"           % btag_mode_;
    std::cout << boost::format(param_fmt()) % "bfake_mode"          % bfake_mode_;
    std::cout << boost::format(param_fmt()) % "do_topquark_weights" % do_topquark_weights_;
    std::cout << boost::format(param_fmt()) % "do_top_jeteta_weights" % do_top_jeteta_weights_;
    std::cout << boost::format(param_fmt()) % "do_tau_fake_weights" % do_tau_fake_weights_;
    std::cout << boost::format(param_fmt()) % "do_tau_id_weights"   % do_tau_id_weights_;
    std::cout << boost::format(param_fmt()) % "do_tau_id_sf"        % do_tau_id_sf_;
    std::cout << boost::format(param_fmt()) % "do_em_qcd_weights"   % do_em_qcd_weights_;
    std::cout << boost::format(param_fmt()) % "jets_label"          % jets_label_;
    std::cout << boost::format(param_fmt()) % "btag_label"          % btag_label_;
    std::cout << boost::format(param_fmt()) % "ditau_label"         % ditau_label_;
    std::cout << boost::format(param_fmt()) % "scalefactor_file"    % scalefactor_file_;
    std::cout << boost::format(param_fmt()) % "scalefactor_file_ggh" % scalefactor_file_ggh_;

    if (do_tau_fake_weights_) {
     tau_fake_weights_ = new TF1("tau_fake_weights","(1.15743)-(0.00736136*x)+(4.3699e-05*x*x)-(1.188e-07*x*x*x)",0,200); 
    }

    if (ggh_mass_ != "" && mc_ == mc::fall11_42X) {
      // if (ggh_mass_ == "90" || ggh_mass_ == "95" ||
      //     ggh_mass_ == "100" || ggh_mass_ == "105") ggh_mass_ = "110";
      // if (ggh_mass_ == "150" || ggh_mass_ == "155" || ggh_mass_ == "160") ggh_mass_ = "145";
      // std::string file = "input/ggh_weights/weight_ptH_"+ggh_mass_+".root";
      std::string file = "input/ggh_weights/HRes_weight_pTH_mH125_7TeV.root";
      std::cout << boost::format(param_fmt()) % "higgs_pt_weights" % file;
      ggh_weights_ = new TFile(file.c_str());
      ggh_weights_->cd();
      ggh_hist_ = (TH1F*)gDirectory->Get("Nominal");
      ggh_hist_up_ = (TH1F*)gDirectory->Get("Up");
      ggh_hist_down_ = (TH1F*)gDirectory->Get("Down");
      // gDirectory->cd("powheg_weight");
      // ggh_hist_ = (TH1F*)gDirectory->Get(("weight_hqt_fehipro_fit_"+ggh_mass_).c_str());
    }
    if (ggh_mass_ != "" && mc_ == mc::summer12_53X) {
      if (  ggh_mass_ ==  "90"  || ggh_mass_ ==  "95" || ggh_mass_ == "100"
         || ggh_mass_ == "105"  || ggh_mass_ == "110" ) ggh_mass_ = "100";
      if (  ggh_mass_ == "115"  || ggh_mass_ ==  "120" || ggh_mass_ == "125"
         || ggh_mass_ == "130"  || ggh_mass_ == "135" ) ggh_mass_ = "125";
      if (  ggh_mass_ == "140"  || ggh_mass_ == "145" || ggh_mass_ == "150"
         || ggh_mass_ == "155"  || ggh_mass_ == "160" ) ggh_mass_ = "150";
      std::string file = "input/ggh_weights/HRes_weight_pTH_mH"+ggh_mass_+"_8TeV.root";
      std::cout << boost::format(param_fmt()) % "higgs_pt_weights" % file;
      ggh_weights_ = new TFile(file.c_str());
      ggh_weights_->cd();
      ggh_hist_ = (TH1F*)gDirectory->Get("Nominal");
      ggh_hist_up_ = (TH1F*)gDirectory->Get("Up");
      ggh_hist_down_ = (TH1F*)gDirectory->Get("Down");
    }

    if (do_emu_e_fakerates_ || do_emu_m_fakerates_) {
      std::string electron_fr_file, muon_fr_file;
      if (era_ == era::data_2012_rereco) {
        electron_fr_file  = "input/emu_fakerate/ElectronFakeRate_2012_19ifb_rereco.root";
        muon_fr_file      = "input/emu_fakerate/MuonFakeRate_2012_19ifb_rereco.root";
      } else {
        electron_fr_file  = "input/emu_fakerate/ElectronFakeRate_2011.root";
        muon_fr_file      = "input/emu_fakerate/MuonFakeRate_2011.root";
      }    
      std::cout << boost::format(param_fmt()) % "electron_fr_file"  % electron_fr_file;
      std::cout << boost::format(param_fmt()) % "muon_fr_file"      % muon_fr_file;
      ElectronFRFile = new TFile(electron_fr_file.c_str());
      MuonFRFile = new TFile(muon_fr_file.c_str());
      ElectronFakeRateHist_PtEta = (mithep::TH2DAsymErr*)(ElectronFRFile->Get("ElectronFakeRateDenominatorV4_Ele8CaloIdLCaloIsoVLCombinedSample_ptThreshold35_PtEta"));
      if (mc_ == mc::fall11_42X) {
        MuonFakeRateHist_PtEta = (mithep::TH2DAsymErr*)(MuonFRFile->Get("MuonFakeRateDenominatorV6_Mu8PtCombinedSample_ptThreshold25_PtEta"));        
      } else {
        MuonFakeRateHist_PtEta = (mithep::TH2DAsymErr*)(MuonFRFile->Get("MuonFakeRateDenominatorV6_Mu8PtCombinedSample_ptThreshold25_PtEta"));        
      }
      ElectronFakeRateHist_PtEta->SetDirectory(0);
      MuonFakeRateHist_PtEta->SetDirectory(0);
    }

    if (do_nnlops_weights_){

      // Retrieve file with TGraphs of weights
      std::string file = "input/ggh_weights/NNLOPS_reweight.root";
      ggh_weights_ = new TFile(file.c_str());
      ggh_weights_->cd();
      ggh_mg_0jet_ = (TGraph*)gDirectory->Get("gr_NNLOPSratio_pt_mcatnlo_0jet");
      ggh_mg_1jet_ = (TGraph*)gDirectory->Get("gr_NNLOPSratio_pt_mcatnlo_1jet");
      ggh_mg_2jet_ = (TGraph*)gDirectory->Get("gr_NNLOPSratio_pt_mcatnlo_2jet");
      ggh_mg_3jet_ = (TGraph*)gDirectory->Get("gr_NNLOPSratio_pt_mcatnlo_3jet");

      ggh_ph_0jet_ = (TGraph*)gDirectory->Get("gr_NNLOPSratio_pt_powheg_0jet");
      ggh_ph_1jet_ = (TGraph*)gDirectory->Get("gr_NNLOPSratio_pt_powheg_1jet");
      ggh_ph_2jet_ = (TGraph*)gDirectory->Get("gr_NNLOPSratio_pt_powheg_2jet");
      ggh_ph_3jet_ = (TGraph*)gDirectory->Get("gr_NNLOPSratio_pt_powheg_3jet");

      ggh_weights_->Close();
   }

    if(scalefactor_file_!="" && (mc_ == mc::mc2018 || mc_ == mc::mc2017 || mc_ == mc::mcleg2016)) {
      // putting new definitions of all function here for full run2 analyses to avoid the mess/confusion 
      // for now just for 2018 but will update for 2017 and 2016 legacy soon
      TFile f(scalefactor_file_.c_str());
      w_ = std::shared_ptr<RooWorkspace>((RooWorkspace*)gDirectory->Get("w"));;
      f.Close();

      if (scalefactor_file_ggh_ != "") {
        TFile f_ggh(scalefactor_file_ggh_.c_str());
        w_ggh_ = std::shared_ptr<RooWorkspace>((RooWorkspace*)gDirectory->Get("w"));;
        f_ggh.Close();
      }

      // tracking corrections for electrons and muons
      fns_["e_trk_ratio"] = std::shared_ptr<RooFunctor>(
          w_->function("e_trk_ratio")->functor(w_->argSet("e_pt,e_eta")));
      fns_["e_trk_embed_ratio"] = std::shared_ptr<RooFunctor>(
          w_->function("e_trk_embed_ratio")->functor(w_->argSet("e_pt,e_eta")));
      fns_["m_trk_ratio"] = std::shared_ptr<RooFunctor>(
          w_->function("m_trk_ratio")->functor(w_->argSet("m_eta")));

      // triggers for muon legs in mt and zmm channels
      
      fns_["m_trg_binned_mc"] = std::shared_ptr<RooFunctor>(  w_->function("m_trg_binned_ic_mc")->functor(w_->argSet("m_pt,m_eta,m_iso")));
      fns_["m_trg_binned_data"] = std::shared_ptr<RooFunctor>(
         w_->function("m_trg_binned_ic_data")->functor(w_->argSet("m_pt,m_eta,m_iso")));
      fns_["m_trg_binned_embed"] = std::shared_ptr<RooFunctor>(
         w_->function("m_trg_binned_ic_embed")->functor(w_->argSet("m_pt,m_eta,m_iso")));
      if(mc_ == mc::mcleg2016) {
        fns_["m_crosstrg_data"] = std::shared_ptr<RooFunctor>(
           w_->function("m_trg_19_binned_ic_data")->functor(w_->argSet("m_pt,m_eta,m_iso")));
        fns_["m_crosstrg_mc"] = std::shared_ptr<RooFunctor>(
           w_->function("m_trg_19_binned_ic_mc")->functor(w_->argSet("m_pt,m_eta,m_iso")));
        fns_["m_crosstrg_embed"] = std::shared_ptr<RooFunctor>(
           w_->function("m_trg_19_binned_ic_embed")->functor(w_->argSet("m_pt,m_eta,m_iso")));
      }
      if(mc_ == mc::mc2017 || mc_ == mc::mc2018) {
        fns_["m_crosstrg_data"] = std::shared_ptr<RooFunctor>(
           w_->function("m_trg_20_binned_ic_data")->functor(w_->argSet("m_pt,m_eta,m_iso")));
        fns_["m_crosstrg_mc"] = std::shared_ptr<RooFunctor>(
           w_->function("m_trg_20_binned_ic_mc")->functor(w_->argSet("m_pt,m_eta,m_iso")));
        fns_["m_crosstrg_embed"] = std::shared_ptr<RooFunctor>(
           w_->function("m_trg_20_binned_ic_embed")->functor(w_->argSet("m_pt,m_eta,m_iso")));
      }

      fns_["e_trg_binned_mc"] = std::shared_ptr<RooFunctor>(
         w_->function("e_trg_binned_ic_mc")->functor(w_->argSet("e_pt,e_eta,e_iso")));
      fns_["e_trg_binned_data"] = std::shared_ptr<RooFunctor>(
         w_->function("e_trg_binned_ic_data")->functor(w_->argSet("e_pt,e_eta,e_iso")));
      fns_["e_trg_binned_embed"] = std::shared_ptr<RooFunctor>(
         w_->function("e_trg_binned_ic_embed")->functor(w_->argSet("e_pt,e_eta,e_iso")));
      if(mc_ == mc::mc2017 || mc_ == mc::mc2018) {
        fns_["e_crosstrg_mc"] = std::shared_ptr<RooFunctor>(
           w_->function("e_trg_24_binned_ic_mc")->functor(w_->argSet("e_pt,e_eta,e_iso")));
        fns_["e_crosstrg_data"] = std::shared_ptr<RooFunctor>(
           w_->function("e_trg_24_binned_ic_data")->functor(w_->argSet("e_pt,e_eta,e_iso")));
        fns_["e_crosstrg_embed"] = std::shared_ptr<RooFunctor>(
           w_->function("e_trg_24_binned_ic_embed")->functor(w_->argSet("e_pt,e_eta,e_iso")));
      }

      // triggers for electron and muon legs in em channel 
      fns_["e_trg_binned_12_mc"] = std::shared_ptr<RooFunctor>(
         w_->function("e_trg_12_binned_ic_mc")->functor(w_->argSet("e_pt,e_eta,e_iso")));
      fns_["e_trg_binned_12_data"] = std::shared_ptr<RooFunctor>(
         w_->function("e_trg_12_binned_ic_data")->functor(w_->argSet("e_pt,e_eta,e_iso")));
       fns_["e_trg_binned_12_embed"] = std::shared_ptr<RooFunctor>(
          w_->function("e_trg_12_binned_ic_embed")->functor(w_->argSet("e_pt,e_eta,e_iso")));
      fns_["e_trg_binned_23_mc"] = std::shared_ptr<RooFunctor>(
         w_->function("e_trg_23_binned_ic_mc")->functor(w_->argSet("e_pt,e_eta,e_iso")));
      fns_["e_trg_binned_23_data"] = std::shared_ptr<RooFunctor>(
         w_->function("e_trg_23_binned_ic_data")->functor(w_->argSet("e_pt,e_eta,e_iso")));
       fns_["e_trg_binned_23_embed"] = std::shared_ptr<RooFunctor>(
          w_->function("e_trg_23_binned_ic_embed")->functor(w_->argSet("e_pt,e_eta,e_iso")));
      fns_["m_trg_binned_8_mc"] = std::shared_ptr<RooFunctor>(
         w_->function("m_trg_8_binned_ic_mc")->functor(w_->argSet("m_pt,m_eta,m_iso")));
      fns_["m_trg_binned_8_data"] = std::shared_ptr<RooFunctor>(
         w_->function("m_trg_8_binned_ic_data")->functor(w_->argSet("m_pt,m_eta,m_iso")));
       fns_["m_trg_binned_8_embed"] = std::shared_ptr<RooFunctor>(
          w_->function("m_trg_8_binned_ic_embed")->functor(w_->argSet("m_pt,m_eta,m_iso")));
      fns_["m_trg_binned_23_mc"] = std::shared_ptr<RooFunctor>(
         w_->function("m_trg_23_binned_ic_mc")->functor(w_->argSet("m_pt,m_eta,m_iso")));
      fns_["m_trg_binned_23_data"] = std::shared_ptr<RooFunctor>(
         w_->function("m_trg_23_binned_ic_data")->functor(w_->argSet("m_pt,m_eta,m_iso")));
       fns_["m_trg_binned_23_embed"] = std::shared_ptr<RooFunctor>(
          w_->function("m_trg_23_binned_ic_embed")->functor(w_->argSet("m_pt,m_eta,m_iso")));
   
      // triggers for tau legs in et, mt and tt channels
      fns_["t_trg_30_embed_data"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_mediumDeepTau_etau_data")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
      fns_["t_trg_30_data"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_pog_deeptau_medium_etau_data")->functor(w_->argSet("t_pt,t_dm")));
      fns_["t_trg_30_mc"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_pog_deeptau_medium_etau_mc")->functor(w_->argSet("t_pt,t_dm")));
      fns_["t_trg_30_embed"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_mediumDeepTau_etau_embed")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
      fns_["t_trg_30_ratio"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_pog_deeptau_medium_etau_ratio")->functor(w_->argSet("t_pt,t_dm")));
      fns_["t_trg_30_embed_ratio"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_mediumDeepTau_etau_embed_ratio")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));

      fns_["t_trg_27_embed_data"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_mediumDeepTau_mutau_data")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
      fns_["t_trg_27_data"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_pog_deeptau_medium_mutau_data")->functor(w_->argSet("t_pt,t_dm")));
      fns_["t_trg_27_mc"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_pog_deeptau_medium_mutau_mc")->functor(w_->argSet("t_pt,t_dm")));
      fns_["t_trg_27_embed"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_mediumDeepTau_mutau_embed")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
      fns_["t_trg_27_ratio"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_pog_deeptau_medium_mutau_ratio")->functor(w_->argSet("t_pt,t_dm")));
      fns_["t_trg_27_embed_ratio"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_mediumDeepTau_mutau_embed_ratio")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));


      fns_["t_trg_35_ratio"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_pog_deeptau_medium_ditau_ratio")->functor(w_->argSet("t_pt,t_dm")));
      fns_["t_trg_35_embed_ratio"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_mediumDeepTau_ditau_embed_ratio")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
      fns_["t_trg_35_embed"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_mediumDeepTau_ditau_embed")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));

      fns_["t_trg_27_ratio_dm0_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_pog_deeptau_medium_mutau_ratio_dm0_up")->functor(w_->argSet("t_pt,t_dm")));
      fns_["t_trg_27_ratio_dm1_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_pog_deeptau_medium_mutau_ratio_dm1_up")->functor(w_->argSet("t_pt,t_dm")));
      fns_["t_trg_27_ratio_dm10_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_pog_deeptau_medium_mutau_ratio_dm10_up")->functor(w_->argSet("t_pt,t_dm")));
      fns_["t_trg_27_ratio_dm11_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_pog_deeptau_medium_mutau_ratio_dm11_up")->functor(w_->argSet("t_pt,t_dm")));
      fns_["t_trg_27_ratio_dm0_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_pog_deeptau_medium_mutau_ratio_dm0_down")->functor(w_->argSet("t_pt,t_dm")));
      fns_["t_trg_27_ratio_dm1_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_pog_deeptau_medium_mutau_ratio_dm1_down")->functor(w_->argSet("t_pt,t_dm")));
      fns_["t_trg_27_ratio_dm10_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_pog_deeptau_medium_mutau_ratio_dm10_down")->functor(w_->argSet("t_pt,t_dm")));
      fns_["t_trg_27_ratio_dm11_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_pog_deeptau_medium_mutau_ratio_dm11_down")->functor(w_->argSet("t_pt,t_dm")));

      fns_["t_trg_27_embed_ratio_dm0_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_mediumDeepTau_mutau_embed_ratio_dm0_up")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
      fns_["t_trg_27_embed_ratio_dm1_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_mediumDeepTau_mutau_embed_ratio_dm1_up")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
      fns_["t_trg_27_embed_ratio_dm10_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_mediumDeepTau_mutau_embed_ratio_dm10_up")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
      fns_["t_trg_27_embed_ratio_dm11_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_mediumDeepTau_mutau_embed_ratio_dm11_up")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
      fns_["t_trg_27_embed_ratio_dm0_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_mediumDeepTau_mutau_embed_ratio_dm0_down")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
      fns_["t_trg_27_embed_ratio_dm1_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_mediumDeepTau_mutau_embed_ratio_dm1_down")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
      fns_["t_trg_27_embed_ratio_dm10_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_mediumDeepTau_mutau_embed_ratio_dm10_down")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
      fns_["t_trg_27_embed_ratio_dm11_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_mediumDeepTau_mutau_embed_ratio_dm11_down")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));

      fns_["t_trg_30_ratio_dm0_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_pog_deeptau_medium_etau_ratio_dm0_up")->functor(w_->argSet("t_pt,t_dm")));
      fns_["t_trg_30_ratio_dm1_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_pog_deeptau_medium_etau_ratio_dm1_up")->functor(w_->argSet("t_pt,t_dm")));
      fns_["t_trg_30_ratio_dm10_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_pog_deeptau_medium_etau_ratio_dm10_up")->functor(w_->argSet("t_pt,t_dm")));
      fns_["t_trg_30_ratio_dm11_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_pog_deeptau_medium_etau_ratio_dm11_up")->functor(w_->argSet("t_pt,t_dm")));
      fns_["t_trg_30_ratio_dm0_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_pog_deeptau_medium_etau_ratio_dm0_down")->functor(w_->argSet("t_pt,t_dm")));
      fns_["t_trg_30_ratio_dm1_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_pog_deeptau_medium_etau_ratio_dm1_down")->functor(w_->argSet("t_pt,t_dm")));
      fns_["t_trg_30_ratio_dm10_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_pog_deeptau_medium_etau_ratio_dm10_down")->functor(w_->argSet("t_pt,t_dm")));
      fns_["t_trg_30_ratio_dm11_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_pog_deeptau_medium_etau_ratio_dm11_down")->functor(w_->argSet("t_pt,t_dm")));

      fns_["t_trg_30_embed_ratio_dm0_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_mediumDeepTau_etau_embed_ratio_dm0_up")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
      fns_["t_trg_30_embed_ratio_dm1_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_mediumDeepTau_etau_embed_ratio_dm1_up")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
      fns_["t_trg_30_embed_ratio_dm10_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_mediumDeepTau_etau_embed_ratio_dm10_up")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
      fns_["t_trg_30_embed_ratio_dm11_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_mediumDeepTau_etau_embed_ratio_dm11_up")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
      fns_["t_trg_30_embed_ratio_dm0_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_mediumDeepTau_etau_embed_ratio_dm0_down")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
      fns_["t_trg_30_embed_ratio_dm1_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_mediumDeepTau_etau_embed_ratio_dm1_down")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
      fns_["t_trg_30_embed_ratio_dm10_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_mediumDeepTau_etau_embed_ratio_dm10_down")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
      fns_["t_trg_30_embed_ratio_dm11_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_mediumDeepTau_etau_embed_ratio_dm11_down")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));

      fns_["t_trg_35_ratio_dm0_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_pog_deeptau_medium_ditau_ratio_dm0_up")->functor(w_->argSet("t_pt,t_dm")));
      fns_["t_trg_35_ratio_dm1_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_pog_deeptau_medium_ditau_ratio_dm1_up")->functor(w_->argSet("t_pt,t_dm")));
      fns_["t_trg_35_ratio_dm10_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_pog_deeptau_medium_ditau_ratio_dm10_up")->functor(w_->argSet("t_pt,t_dm")));
      fns_["t_trg_35_ratio_dm11_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_pog_deeptau_medium_ditau_ratio_dm11_up")->functor(w_->argSet("t_pt,t_dm")));
      fns_["t_trg_35_ratio_dm0_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_pog_deeptau_medium_ditau_ratio_dm0_down")->functor(w_->argSet("t_pt,t_dm")));
      fns_["t_trg_35_ratio_dm1_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_pog_deeptau_medium_ditau_ratio_dm1_down")->functor(w_->argSet("t_pt,t_dm")));
      fns_["t_trg_35_ratio_dm10_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_pog_deeptau_medium_ditau_ratio_dm10_down")->functor(w_->argSet("t_pt,t_dm")));
      fns_["t_trg_35_ratio_dm11_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_pog_deeptau_medium_ditau_ratio_dm11_down")->functor(w_->argSet("t_pt,t_dm")));

      fns_["t_trg_35_embed_ratio_dm0_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_mediumDeepTau_ditau_embed_ratio_dm0_up")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
      fns_["t_trg_35_embed_ratio_dm1_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_mediumDeepTau_ditau_embed_ratio_dm1_up")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
      fns_["t_trg_35_embed_ratio_dm10_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_mediumDeepTau_ditau_embed_ratio_dm10_up")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
      fns_["t_trg_35_embed_ratio_dm11_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_mediumDeepTau_ditau_embed_ratio_dm11_up")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
      fns_["t_trg_35_embed_ratio_dm0_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_mediumDeepTau_ditau_embed_ratio_dm0_down")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
      fns_["t_trg_35_embed_ratio_dm1_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_mediumDeepTau_ditau_embed_ratio_dm1_down")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
      fns_["t_trg_35_embed_ratio_dm10_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_mediumDeepTau_ditau_embed_ratio_dm10_down")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
      fns_["t_trg_35_embed_ratio_dm11_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_mediumDeepTau_ditau_embed_ratio_dm11_down")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));

      // Mohammads trigger SFs
      //t_trg_ic_deeptau_medium_mvadm_ditau_ratio_mvadm1_down

      fns_["t_trg_27_ic_ratio"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mutau_ratio")->functor(w_->argSet("t_pt,t_dm")));
      fns_["t_trg_27_ic_embed_ratio"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mutau_embed_ratio")->functor(w_->argSet("t_pt,t_dm")));

      fns_["t_trg_27_ic_mvadm_ratio"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_mutau_ratio")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_trg_27_ic_mvadm_embed_ratio"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_mutau_embed_ratio")->functor(w_->argSet("t_pt,t_mvadm")));

      fns_["t_trg_27_ic_mvadm_ratio_mvadm0_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_mutau_ratio_mvadm0_up")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_trg_27_ic_mvadm_ratio_mvadm0_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_mutau_ratio_mvadm0_down")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_trg_27_ic_mvadm_ratio_mvadm1_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_mutau_ratio_mvadm1_up")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_trg_27_ic_mvadm_ratio_mvadm1_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_mutau_ratio_mvadm1_down")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_trg_27_ic_mvadm_ratio_mvadm2_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_mutau_ratio_mvadm2_up")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_trg_27_ic_mvadm_ratio_mvadm2_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_mutau_ratio_mvadm2_down")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_trg_27_ic_mvadm_ratio_mvadm10_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_mutau_ratio_mvadm10_up")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_trg_27_ic_mvadm_ratio_mvadm10_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_mutau_ratio_mvadm10_down")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_trg_27_ic_mvadm_ratio_mvadm11_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_mutau_ratio_mvadm11_up")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_trg_27_ic_mvadm_ratio_mvadm11_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_mutau_ratio_mvadm11_down")->functor(w_->argSet("t_pt,t_mvadm")));

      fns_["t_trg_27_ic_mvadm_embed_ratio_mvadm0_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_mutau_embed_ratio_mvadm0_up")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_trg_27_ic_mvadm_embed_ratio_mvadm0_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_mutau_embed_ratio_mvadm0_down")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_trg_27_ic_mvadm_embed_ratio_mvadm1_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_mutau_embed_ratio_mvadm1_up")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_trg_27_ic_mvadm_embed_ratio_mvadm1_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_mutau_embed_ratio_mvadm1_down")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_trg_27_ic_mvadm_embed_ratio_mvadm2_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_mutau_embed_ratio_mvadm2_up")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_trg_27_ic_mvadm_embed_ratio_mvadm2_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_mutau_embed_ratio_mvadm2_down")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_trg_27_ic_mvadm_embed_ratio_mvadm10_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_mutau_embed_ratio_mvadm10_up")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_trg_27_ic_mvadm_embed_ratio_mvadm10_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_mutau_embed_ratio_mvadm10_down")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_trg_27_ic_mvadm_embed_ratio_mvadm11_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_mutau_embed_ratio_mvadm11_up")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_trg_27_ic_mvadm_embed_ratio_mvadm11_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_mutau_embed_ratio_mvadm11_down")->functor(w_->argSet("t_pt,t_mvadm")));

      if(mc_ != mc::mcleg2016) {
        fns_["t_trg_30_ic_ratio"] = std::shared_ptr<RooFunctor>(
            w_->function("t_trg_ic_deeptau_medium_etau_ratio")->functor(w_->argSet("t_pt,t_dm")));
        fns_["t_trg_30_ic_embed_ratio"] = std::shared_ptr<RooFunctor>(
            w_->function("t_trg_ic_deeptau_medium_etau_embed_ratio")->functor(w_->argSet("t_pt,t_dm")));

        fns_["t_trg_30_ic_embed_data"] = std::shared_ptr<RooFunctor>(
            w_->function("t_trg_ic_deeptau_medium_etau_embed_data")->functor(w_->argSet("t_pt,t_dm")));
        fns_["t_trg_30_ic_mvadm_ratio"] = std::shared_ptr<RooFunctor>(
            w_->function("t_trg_ic_deeptau_medium_mvadm_etau_ratio")->functor(w_->argSet("t_pt,t_mvadm")));
        fns_["t_trg_30_ic_mvadm_embed_ratio"] = std::shared_ptr<RooFunctor>(
            w_->function("t_trg_ic_deeptau_medium_mvadm_etau_embed_ratio")->functor(w_->argSet("t_pt,t_mvadm")));
        fns_["t_trg_30_ic_mvadm_embed_data"] = std::shared_ptr<RooFunctor>(
            w_->function("t_trg_ic_deeptau_medium_mvadm_etau_embed_data")->functor(w_->argSet("t_pt,t_mvadm")));

        fns_["t_trg_30_ic_mvadm_ratio_mvadm0_up"] = std::shared_ptr<RooFunctor>(
            w_->function("t_trg_ic_deeptau_medium_mvadm_etau_ratio_mvadm0_up")->functor(w_->argSet("t_pt,t_mvadm")));
        fns_["t_trg_30_ic_mvadm_ratio_mvadm0_down"] = std::shared_ptr<RooFunctor>(
            w_->function("t_trg_ic_deeptau_medium_mvadm_etau_ratio_mvadm0_down")->functor(w_->argSet("t_pt,t_mvadm")));
        fns_["t_trg_30_ic_mvadm_ratio_mvadm1_up"] = std::shared_ptr<RooFunctor>(
            w_->function("t_trg_ic_deeptau_medium_mvadm_etau_ratio_mvadm1_up")->functor(w_->argSet("t_pt,t_mvadm")));
        fns_["t_trg_30_ic_mvadm_ratio_mvadm1_down"] = std::shared_ptr<RooFunctor>(
            w_->function("t_trg_ic_deeptau_medium_mvadm_etau_ratio_mvadm1_down")->functor(w_->argSet("t_pt,t_mvadm")));
        fns_["t_trg_30_ic_mvadm_ratio_mvadm2_up"] = std::shared_ptr<RooFunctor>(
            w_->function("t_trg_ic_deeptau_medium_mvadm_etau_ratio_mvadm2_up")->functor(w_->argSet("t_pt,t_mvadm")));
        fns_["t_trg_30_ic_mvadm_ratio_mvadm2_down"] = std::shared_ptr<RooFunctor>(
            w_->function("t_trg_ic_deeptau_medium_mvadm_etau_ratio_mvadm2_down")->functor(w_->argSet("t_pt,t_mvadm")));
        fns_["t_trg_30_ic_mvadm_ratio_mvadm10_up"] = std::shared_ptr<RooFunctor>(
            w_->function("t_trg_ic_deeptau_medium_mvadm_etau_ratio_mvadm10_up")->functor(w_->argSet("t_pt,t_mvadm")));
        fns_["t_trg_30_ic_mvadm_ratio_mvadm10_down"] = std::shared_ptr<RooFunctor>(
            w_->function("t_trg_ic_deeptau_medium_mvadm_etau_ratio_mvadm10_down")->functor(w_->argSet("t_pt,t_mvadm")));
        fns_["t_trg_30_ic_mvadm_ratio_mvadm11_up"] = std::shared_ptr<RooFunctor>(
            w_->function("t_trg_ic_deeptau_medium_mvadm_etau_ratio_mvadm11_up")->functor(w_->argSet("t_pt,t_mvadm")));
        fns_["t_trg_30_ic_mvadm_ratio_mvadm11_down"] = std::shared_ptr<RooFunctor>(
            w_->function("t_trg_ic_deeptau_medium_mvadm_etau_ratio_mvadm11_down")->functor(w_->argSet("t_pt,t_mvadm")));

        fns_["t_trg_30_ic_mvadm_embed_ratio_mvadm0_up"] = std::shared_ptr<RooFunctor>(
            w_->function("t_trg_ic_deeptau_medium_mvadm_etau_embed_ratio_mvadm0_up")->functor(w_->argSet("t_pt,t_mvadm")));
        fns_["t_trg_30_ic_mvadm_embed_ratio_mvadm0_down"] = std::shared_ptr<RooFunctor>(
            w_->function("t_trg_ic_deeptau_medium_mvadm_etau_embed_ratio_mvadm0_down")->functor(w_->argSet("t_pt,t_mvadm")));
        fns_["t_trg_30_ic_mvadm_embed_ratio_mvadm1_up"] = std::shared_ptr<RooFunctor>(
            w_->function("t_trg_ic_deeptau_medium_mvadm_etau_embed_ratio_mvadm1_up")->functor(w_->argSet("t_pt,t_mvadm")));
        fns_["t_trg_30_ic_mvadm_embed_ratio_mvadm1_down"] = std::shared_ptr<RooFunctor>(
            w_->function("t_trg_ic_deeptau_medium_mvadm_etau_embed_ratio_mvadm1_down")->functor(w_->argSet("t_pt,t_mvadm")));
        fns_["t_trg_30_ic_mvadm_embed_ratio_mvadm2_up"] = std::shared_ptr<RooFunctor>(
            w_->function("t_trg_ic_deeptau_medium_mvadm_etau_embed_ratio_mvadm2_up")->functor(w_->argSet("t_pt,t_mvadm")));
        fns_["t_trg_30_ic_mvadm_embed_ratio_mvadm2_down"] = std::shared_ptr<RooFunctor>(
            w_->function("t_trg_ic_deeptau_medium_mvadm_etau_embed_ratio_mvadm2_down")->functor(w_->argSet("t_pt,t_mvadm")));
        fns_["t_trg_30_ic_mvadm_embed_ratio_mvadm10_up"] = std::shared_ptr<RooFunctor>(
            w_->function("t_trg_ic_deeptau_medium_mvadm_etau_embed_ratio_mvadm10_up")->functor(w_->argSet("t_pt,t_mvadm")));
        fns_["t_trg_30_ic_mvadm_embed_ratio_mvadm10_down"] = std::shared_ptr<RooFunctor>(
            w_->function("t_trg_ic_deeptau_medium_mvadm_etau_embed_ratio_mvadm10_down")->functor(w_->argSet("t_pt,t_mvadm")));
        fns_["t_trg_30_ic_mvadm_embed_ratio_mvadm11_up"] = std::shared_ptr<RooFunctor>(
            w_->function("t_trg_ic_deeptau_medium_mvadm_etau_embed_ratio_mvadm11_up")->functor(w_->argSet("t_pt,t_mvadm")));
        fns_["t_trg_30_ic_mvadm_embed_ratio_mvadm11_down"] = std::shared_ptr<RooFunctor>(
            w_->function("t_trg_ic_deeptau_medium_mvadm_etau_embed_ratio_mvadm11_down")->functor(w_->argSet("t_pt,t_mvadm")));
      }

      fns_["t_trg_35_ic_ratio"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_ditau_ratio")->functor(w_->argSet("t_pt,t_dm")));
      fns_["t_trg_35_ic_embed_ratio"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_ditau_embed_ratio")->functor(w_->argSet("t_pt,t_dm")));

      fns_["t_trg_35_ic_mvadm_ratio"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_ditau_ratio")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_trg_35_ic_mvadm_embed_ratio"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_ditau_embed_ratio")->functor(w_->argSet("t_pt,t_mvadm")));

      fns_["t_trg_35_ic_mvadm_ratio_mvadm0_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_ditau_ratio_mvadm0_up")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_trg_35_ic_mvadm_ratio_mvadm0_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_ditau_ratio_mvadm0_down")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_trg_35_ic_mvadm_embed_ratio_mvadm0_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_ditau_embed_ratio_mvadm0_up")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_trg_35_ic_mvadm_embed_ratio_mvadm0_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_ditau_embed_ratio_mvadm0_down")->functor(w_->argSet("t_pt,t_mvadm")));

      fns_["t_trg_35_ic_mvadm_ratio_mvadm1_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_ditau_ratio_mvadm1_up")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_trg_35_ic_mvadm_ratio_mvadm1_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_ditau_ratio_mvadm1_down")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_trg_35_ic_mvadm_embed_ratio_mvadm1_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_ditau_embed_ratio_mvadm1_up")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_trg_35_ic_mvadm_embed_ratio_mvadm1_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_ditau_embed_ratio_mvadm1_down")->functor(w_->argSet("t_pt,t_mvadm")));

      fns_["t_trg_35_ic_mvadm_ratio_mvadm2_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_ditau_ratio_mvadm2_up")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_trg_35_ic_mvadm_ratio_mvadm2_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_ditau_ratio_mvadm2_down")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_trg_35_ic_mvadm_embed_ratio_mvadm2_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_ditau_embed_ratio_mvadm2_up")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_trg_35_ic_mvadm_embed_ratio_mvadm2_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_ditau_embed_ratio_mvadm2_down")->functor(w_->argSet("t_pt,t_mvadm")));

      fns_["t_trg_35_ic_mvadm_ratio_mvadm10_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_ditau_ratio_mvadm10_up")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_trg_35_ic_mvadm_ratio_mvadm10_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_ditau_ratio_mvadm10_down")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_trg_35_ic_mvadm_embed_ratio_mvadm10_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_ditau_embed_ratio_mvadm10_up")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_trg_35_ic_mvadm_embed_ratio_mvadm10_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_ditau_embed_ratio_mvadm10_down")->functor(w_->argSet("t_pt,t_mvadm")));

      fns_["t_trg_35_ic_mvadm_ratio_mvadm11_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_ditau_ratio_mvadm11_up")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_trg_35_ic_mvadm_ratio_mvadm11_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_ditau_ratio_mvadm11_down")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_trg_35_ic_mvadm_embed_ratio_mvadm11_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_ditau_embed_ratio_mvadm11_up")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_trg_35_ic_mvadm_embed_ratio_mvadm11_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_ditau_embed_ratio_mvadm11_down")->functor(w_->argSet("t_pt,t_mvadm")));

      // electron id/iso
      fns_["e_idiso_embed_ratio"] = std::shared_ptr<RooFunctor>(
          w_->function("e_idiso_binned_ic_embed_ratio")->functor(w_->argSet("e_pt,e_eta,e_iso")));
      fns_["e_idiso_ratio"] = std::shared_ptr<RooFunctor>(
          w_->function("e_idiso_binned_ic_ratio")->functor(w_->argSet("e_pt,e_eta,e_iso"))); 

      // muon id/iso
      fns_["m_idiso_embed_ratio"] = std::shared_ptr<RooFunctor>(
          w_->function("m_idiso_binned_ic_embed_ratio")->functor(w_->argSet("m_pt,m_eta,m_iso")));
       fns_["m_idiso_ratio"] = std::shared_ptr<RooFunctor>(
        w_->function("m_idiso_binned_ic_ratio")->functor(w_->argSet("m_pt,m_eta,m_iso")));
      fns_["m_id_ratio"] = std::shared_ptr<RooFunctor>(
          w_->function("m_id_ic_ratio")->functor(w_->argSet("m_pt,m_eta")));
      fns_["m_id_embed_ratio"] = std::shared_ptr<RooFunctor>(
          w_->function("m_id_ic_embed_ratio")->functor(w_->argSet("m_pt,m_eta")));
      fns_["m_looseiso_ratio"] = std::shared_ptr<RooFunctor>(
          w_->function("m_looseiso_binned_ic_ratio")->functor(w_->argSet("m_pt,m_eta,m_iso")));
      fns_["m_looseiso_embed_ratio"] = std::shared_ptr<RooFunctor>(
          w_->function("m_looseiso_binned_ic_embed_ratio")->functor(w_->argSet("m_pt,m_eta,m_iso")));
      fns_["m_sel_idEmb_ratio"] = std::shared_ptr<RooFunctor>(
           w_->function("m_sel_id_ic_ratio")->functor(w_->argSet("gt_eta,gt_pt")));
      fns_["m_sel_trg_ratio"] = std::shared_ptr<RooFunctor>(
          w_->function("m_sel_trg_ic_ratio")->functor(w_->argSet("gt1_pt,gt1_eta,gt2_pt,gt2_eta")));

      // tau id
      fns_["t_id_pt_tight"] = std::shared_ptr<RooFunctor>(
          w_->function("t_id_pt_tight")->functor(w_->argSet("t_pt")));
      fns_["t_id_dm_tight"] = std::shared_ptr<RooFunctor>(
          w_->function("t_id_dm_tight")->functor(w_->argSet("t_dm")));
      fns_["t_id_dm_vloose"] = std::shared_ptr<RooFunctor>(
        w_->function("t_id_dm_vloose")->functor(w_->argSet("t_dm")));
      fns_["t_deeptauid_pt_medium"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_pt_medium")->functor(w_->argSet("t_pt")));

      fns_["t_deeptauid_pt_vvvloose"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_pt_vvvloose")->functor(w_->argSet("t_pt")));
      fns_["t_deeptauid_dm_medium"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_dm_medium")->functor(w_->argSet("t_dm")));
      fns_["t_deeptauid_dm_vvvloose"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_dm_vvvloose")->functor(w_->argSet("t_dm")));


      fns_["t_deeptauid_pt_medium_bin1_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_pt_medium_bin1_up")->functor(w_->argSet("t_pt")));
      fns_["t_deeptauid_pt_medium_bin2_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_pt_medium_bin2_up")->functor(w_->argSet("t_pt")));
      fns_["t_deeptauid_pt_medium_bin3_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_pt_medium_bin3_up")->functor(w_->argSet("t_pt")));
      fns_["t_deeptauid_pt_medium_bin4_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_pt_medium_bin4_up")->functor(w_->argSet("t_pt")));
      fns_["t_deeptauid_pt_medium_bin5_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_pt_medium_bin5_up")->functor(w_->argSet("t_pt")));

      fns_["t_deeptauid_pt_medium_bin1_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_pt_medium_bin1_down")->functor(w_->argSet("t_pt")));
      fns_["t_deeptauid_pt_medium_bin2_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_pt_medium_bin2_down")->functor(w_->argSet("t_pt")));
      fns_["t_deeptauid_pt_medium_bin3_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_pt_medium_bin3_down")->functor(w_->argSet("t_pt")));
      fns_["t_deeptauid_pt_medium_bin4_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_pt_medium_bin4_down")->functor(w_->argSet("t_pt")));
      fns_["t_deeptauid_pt_medium_bin5_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_pt_medium_bin5_down")->functor(w_->argSet("t_pt")));

      fns_["t_deeptauid_dm_medium_dm0_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_dm_medium_dm0_up")->functor(w_->argSet("t_dm")));
      fns_["t_deeptauid_dm_medium_dm1_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_dm_medium_dm1_up")->functor(w_->argSet("t_dm")));
      fns_["t_deeptauid_dm_medium_dm10_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_dm_medium_dm10_up")->functor(w_->argSet("t_dm")));
      fns_["t_deeptauid_dm_medium_dm11_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_dm_medium_dm11_up")->functor(w_->argSet("t_dm")));

      fns_["t_deeptauid_dm_medium_dm0_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_dm_medium_dm0_down")->functor(w_->argSet("t_dm")));
      fns_["t_deeptauid_dm_medium_dm1_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_dm_medium_dm1_down")->functor(w_->argSet("t_dm")));
      fns_["t_deeptauid_dm_medium_dm10_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_dm_medium_dm10_down")->functor(w_->argSet("t_dm")));
      fns_["t_deeptauid_dm_medium_dm11_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_dm_medium_dm11_down")->functor(w_->argSet("t_dm")));

      fns_["t_deeptauid_pt_embed_medium"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_pt_embed_medium")->functor(w_->argSet("t_pt")));
      fns_["t_deeptauid_pt_tightvse_embed_medium"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_pt_tightvse_embed_medium")->functor(w_->argSet("t_pt")));
      fns_["t_deeptauid_pt_embed_vvvloose"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_pt_embed_vvvloose")->functor(w_->argSet("t_pt")));
      fns_["t_deeptauid_dm_embed_medium"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_dm_embed_medium")->functor(w_->argSet("t_dm")));
      fns_["t_deeptauid_dm_embed_vvvloose"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_dm_embed_vvvloose")->functor(w_->argSet("t_dm")));


      fns_["t_deeptauid_pt_embed_medium_bin1_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_pt_embed_medium_bin1_up")->functor(w_->argSet("t_pt")));
      fns_["t_deeptauid_pt_embed_medium_bin2_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_pt_embed_medium_bin2_up")->functor(w_->argSet("t_pt")));
      fns_["t_deeptauid_pt_embed_medium_bin3_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_pt_embed_medium_bin3_up")->functor(w_->argSet("t_pt")));
      fns_["t_deeptauid_pt_embed_medium_bin4_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_pt_embed_medium_bin4_up")->functor(w_->argSet("t_pt")));
      fns_["t_deeptauid_pt_embed_medium_bin5_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_pt_embed_medium_bin5_up")->functor(w_->argSet("t_pt")));

      fns_["t_deeptauid_pt_embed_medium_bin1_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_pt_embed_medium_bin1_down")->functor(w_->argSet("t_pt")));
      fns_["t_deeptauid_pt_embed_medium_bin2_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_pt_embed_medium_bin2_down")->functor(w_->argSet("t_pt")));
      fns_["t_deeptauid_pt_embed_medium_bin3_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_pt_embed_medium_bin3_down")->functor(w_->argSet("t_pt")));
      fns_["t_deeptauid_pt_embed_medium_bin4_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_pt_embed_medium_bin4_down")->functor(w_->argSet("t_pt")));
      fns_["t_deeptauid_pt_embed_medium_bin5_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_pt_embed_medium_bin5_down")->functor(w_->argSet("t_pt")));

      fns_["t_deeptauid_dm_embed_medium_dm0_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_dm_embed_medium_dm0_up")->functor(w_->argSet("t_dm")));
      fns_["t_deeptauid_dm_embed_medium_dm1_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_dm_embed_medium_dm1_up")->functor(w_->argSet("t_dm")));
      fns_["t_deeptauid_dm_embed_medium_dm10_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_dm_embed_medium_dm10_up")->functor(w_->argSet("t_dm")));
      fns_["t_deeptauid_dm_embed_medium_dm11_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_dm_embed_medium_dm11_up")->functor(w_->argSet("t_dm")));

      fns_["t_deeptauid_dm_embed_medium_dm0_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_dm_embed_medium_dm0_down")->functor(w_->argSet("t_dm")));
      fns_["t_deeptauid_dm_embed_medium_dm1_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_dm_embed_medium_dm1_down")->functor(w_->argSet("t_dm")));
      fns_["t_deeptauid_dm_embed_medium_dm10_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_dm_embed_medium_dm10_down")->functor(w_->argSet("t_dm")));
      fns_["t_deeptauid_dm_embed_medium_dm11_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_dm_embed_medium_dm11_down")->functor(w_->argSet("t_dm")));

      // tau id in mva-dm bins
//      t_deeptauid_mvadm_embed_medium_down, t_deeptauid_mvadm_%(i)smedium_lowpt_mvadm0_up
//
      fns_["t_deeptauid_mvadm_medium"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_mvadm_medium")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_deeptauid_mvadm_embed_medium"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_mvadm_embed_medium")->functor(w_->argSet("t_pt,t_mvadm")));

      fns_["t_deeptauid_mvadm_medium_lowpt_mvadm0_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_mvadm_medium_lowpt_mvadm0_up")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_deeptauid_mvadm_medium_lowpt_mvadm1_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_mvadm_medium_lowpt_mvadm1_up")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_deeptauid_mvadm_medium_lowpt_mvadm2_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_mvadm_medium_lowpt_mvadm2_up")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_deeptauid_mvadm_medium_lowpt_mvadm10_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_mvadm_medium_lowpt_mvadm10_up")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_deeptauid_mvadm_medium_lowpt_mvadm11_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_mvadm_medium_lowpt_mvadm11_up")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_deeptauid_mvadm_medium_highpt_mvadm0_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_mvadm_medium_highpt_mvadm0_up")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_deeptauid_mvadm_medium_highpt_mvadm1_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_mvadm_medium_highpt_mvadm1_up")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_deeptauid_mvadm_medium_highpt_mvadm2_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_mvadm_medium_highpt_mvadm2_up")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_deeptauid_mvadm_medium_highpt_mvadm10_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_mvadm_medium_highpt_mvadm10_up")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_deeptauid_mvadm_medium_highpt_mvadm11_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_mvadm_medium_highpt_mvadm11_up")->functor(w_->argSet("t_pt,t_mvadm")));

      fns_["t_deeptauid_mvadm_medium_lowpt_mvadm0_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_mvadm_medium_lowpt_mvadm0_down")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_deeptauid_mvadm_medium_lowpt_mvadm1_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_mvadm_medium_lowpt_mvadm1_down")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_deeptauid_mvadm_medium_lowpt_mvadm2_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_mvadm_medium_lowpt_mvadm2_down")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_deeptauid_mvadm_medium_lowpt_mvadm10_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_mvadm_medium_lowpt_mvadm10_down")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_deeptauid_mvadm_medium_lowpt_mvadm11_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_mvadm_medium_lowpt_mvadm11_down")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_deeptauid_mvadm_medium_highpt_mvadm0_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_mvadm_medium_highpt_mvadm0_down")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_deeptauid_mvadm_medium_highpt_mvadm1_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_mvadm_medium_highpt_mvadm1_down")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_deeptauid_mvadm_medium_highpt_mvadm2_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_mvadm_medium_highpt_mvadm2_down")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_deeptauid_mvadm_medium_highpt_mvadm10_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_mvadm_medium_highpt_mvadm10_down")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_deeptauid_mvadm_medium_highpt_mvadm11_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_mvadm_medium_highpt_mvadm11_down")->functor(w_->argSet("t_pt,t_mvadm")));

      fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm0_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_mvadm_embed_medium_lowpt_mvadm0_up")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm1_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_mvadm_embed_medium_lowpt_mvadm1_up")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm2_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_mvadm_embed_medium_lowpt_mvadm2_up")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm10_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_mvadm_embed_medium_lowpt_mvadm10_up")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm11_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_mvadm_embed_medium_lowpt_mvadm11_up")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm0_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_mvadm_embed_medium_highpt_mvadm0_up")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm1_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_mvadm_embed_medium_highpt_mvadm1_up")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm2_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_mvadm_embed_medium_highpt_mvadm2_up")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm10_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_mvadm_embed_medium_highpt_mvadm10_up")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm11_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_mvadm_embed_medium_highpt_mvadm11_up")->functor(w_->argSet("t_pt,t_mvadm")));

      fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm0_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_mvadm_embed_medium_lowpt_mvadm0_down")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm1_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_mvadm_embed_medium_lowpt_mvadm1_down")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm2_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_mvadm_embed_medium_lowpt_mvadm2_down")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm10_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_mvadm_embed_medium_lowpt_mvadm10_down")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm11_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_mvadm_embed_medium_lowpt_mvadm11_down")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm0_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_mvadm_embed_medium_highpt_mvadm0_down")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm1_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_mvadm_embed_medium_highpt_mvadm1_down")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm2_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_mvadm_embed_medium_highpt_mvadm2_down")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm10_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_mvadm_embed_medium_highpt_mvadm10_down")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm11_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_deeptauid_mvadm_embed_medium_highpt_mvadm11_down")->functor(w_->argSet("t_pt,t_mvadm")));

      fns_["t_id_vs_e_eta_vvloose"] = std::shared_ptr<RooFunctor>(
          w_->function("t_id_vs_e_eta_vvloose")->functor(w_->argSet("t_eta")));
      fns_["t_id_vs_e_eta_vvloose_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_id_vs_e_eta_vvloose_up")->functor(w_->argSet("t_eta")));
      fns_["t_id_vs_e_eta_vvloose_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_id_vs_e_eta_vvloose_down")->functor(w_->argSet("t_eta")));
      fns_["t_id_vs_e_eta_tight"] = std::shared_ptr<RooFunctor>(
          w_->function("t_id_vs_e_eta_tight")->functor(w_->argSet("t_eta")));
      fns_["t_id_vs_e_eta_tight_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_id_vs_e_eta_tight_up")->functor(w_->argSet("t_eta")));
      fns_["t_id_vs_e_eta_tight_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_id_vs_e_eta_tight_down")->functor(w_->argSet("t_eta")));

      fns_["t_id_vs_mu_eta_vloose"] = std::shared_ptr<RooFunctor>(
          w_->function("t_id_vs_mu_eta_vloose")->functor(w_->argSet("t_eta")));
      fns_["t_id_vs_mu_eta_vloose_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_id_vs_mu_eta_vloose_up")->functor(w_->argSet("t_eta")));
      fns_["t_id_vs_mu_eta_vloose_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_id_vs_mu_eta_vloose_down")->functor(w_->argSet("t_eta")));
      fns_["t_id_vs_mu_eta_tight"] = std::shared_ptr<RooFunctor>(
          w_->function("t_id_vs_mu_eta_tight")->functor(w_->argSet("t_eta")));
      fns_["t_id_vs_mu_eta_tight_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_id_vs_mu_eta_tight_up")->functor(w_->argSet("t_eta")));
      fns_["t_id_vs_mu_eta_tight_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_id_vs_mu_eta_tight_down")->functor(w_->argSet("t_eta")));

      // zpt reweighting
      fns_["zpt_weight_nom"] = std::shared_ptr<RooFunctor>(
          w_->function("zptmass_weight_nom")->functor(w_->argSet("z_gen_pt,z_gen_mass")));

      // em osss same names for all years
      fns_["em_qcd_osss"] = std::shared_ptr<RooFunctor>(
         w_->function("em_qcd_osss")->functor(w_->argSet("dR,njets,e_pt,m_pt")));
      fns_["em_qcd_osss_extrap_up"] = std::shared_ptr<RooFunctor>(
         w_->function("em_qcd_osss_extrap_up")->functor(w_->argSet("dR,njets,e_pt,m_pt")));
      fns_["em_qcd_osss_extrap_down"] = std::shared_ptr<RooFunctor>(
         w_->function("em_qcd_osss_extrap_down")->functor(w_->argSet("dR,njets,e_pt,m_pt")));

      fns_["em_qcd_osss_stat_0jet_unc1_up"] = std::shared_ptr<RooFunctor>(
         w_->function("em_qcd_osss_stat_0jet_unc1_up")->functor(w_->argSet("dR,njets,e_pt,m_pt")));
      fns_["em_qcd_osss_stat_0jet_unc1_down"] = std::shared_ptr<RooFunctor>(
         w_->function("em_qcd_osss_stat_0jet_unc1_down")->functor(w_->argSet("dR,njets,e_pt,m_pt")));
      fns_["em_qcd_osss_stat_0jet_unc2_up"] = std::shared_ptr<RooFunctor>(
         w_->function("em_qcd_osss_stat_0jet_unc2_up")->functor(w_->argSet("dR,njets,e_pt,m_pt")));
      fns_["em_qcd_osss_stat_0jet_unc2_down"] = std::shared_ptr<RooFunctor>(
         w_->function("em_qcd_osss_stat_0jet_unc2_down")->functor(w_->argSet("dR,njets,e_pt,m_pt")));

      fns_["em_qcd_osss_stat_1jet_unc1_up"] = std::shared_ptr<RooFunctor>(
         w_->function("em_qcd_osss_stat_1jet_unc1_up")->functor(w_->argSet("dR,njets,e_pt,m_pt")));
      fns_["em_qcd_osss_stat_1jet_unc1_down"] = std::shared_ptr<RooFunctor>(
         w_->function("em_qcd_osss_stat_1jet_unc1_down")->functor(w_->argSet("dR,njets,e_pt,m_pt")));
      fns_["em_qcd_osss_stat_1jet_unc2_up"] = std::shared_ptr<RooFunctor>(
         w_->function("em_qcd_osss_stat_1jet_unc2_up")->functor(w_->argSet("dR,njets,e_pt,m_pt")));
      fns_["em_qcd_osss_stat_1jet_unc2_down"] = std::shared_ptr<RooFunctor>(
         w_->function("em_qcd_osss_stat_1jet_unc2_down")->functor(w_->argSet("dR,njets,e_pt,m_pt")));

      fns_["em_qcd_osss_stat_2jet_unc1_up"] = std::shared_ptr<RooFunctor>(
         w_->function("em_qcd_osss_stat_2jet_unc1_up")->functor(w_->argSet("dR,njets,e_pt,m_pt")));
      fns_["em_qcd_osss_stat_2jet_unc1_down"] = std::shared_ptr<RooFunctor>(
         w_->function("em_qcd_osss_stat_2jet_unc1_down")->functor(w_->argSet("dR,njets,e_pt,m_pt")));
      fns_["em_qcd_osss_stat_2jet_unc2_up"] = std::shared_ptr<RooFunctor>(
         w_->function("em_qcd_osss_stat_2jet_unc2_up")->functor(w_->argSet("dR,njets,e_pt,m_pt")));
      fns_["em_qcd_osss_stat_2jet_unc2_down"] = std::shared_ptr<RooFunctor>(
         w_->function("em_qcd_osss_stat_2jet_unc2_down")->functor(w_->argSet("dR,njets,e_pt,m_pt")));

      // MG ggH specific weights
      if(do_quarkmass_higgspt_){
        fns_["ggH_quarkmass_corr"] = std::shared_ptr<RooFunctor>(
                  w_ggh_->function("ggH_quarkmass_corr")->functor(w_ggh_->argSet("HpT")));
        fns_["ggH_quarkmass_corr_up"] = std::shared_ptr<RooFunctor>(
                  w_ggh_->function("ggH_quarkmass_corr_up")->functor(w_ggh_->argSet("HpT")));
        fns_["ggH_quarkmass_corr_down"] = std::shared_ptr<RooFunctor>(
                  w_ggh_->function("ggH_quarkmass_corr_down")->functor(w_ggh_->argSet("HpT")));
        fns_["ggH_fullquarkmass_corr"] = std::shared_ptr<RooFunctor>(
                  w_ggh_->function("ggH_fullquarkmass_corr")->functor(w_ggh_->argSet("HpT")));
      }
      if(do_ps_weights_){
        fns_["ggH_mg_ps_up"] = std::shared_ptr<RooFunctor>(
                  w_ggh_->function("ggH_mg_ps_up")->functor(w_ggh_->argSet("ngenjets,HpT")));
        fns_["ggH_mg_ps_down"] = std::shared_ptr<RooFunctor>(
                  w_ggh_->function("ggH_mg_ps_down")->functor(w_ggh_->argSet("ngenjets,HpT")));
        fns_["ggH_mg_ue_up"] = std::shared_ptr<RooFunctor>(
                  w_ggh_->function("ggH_mg_ue_up")->functor(w_ggh_->argSet("ngenjets")));
        fns_["ggH_mg_ue_down"] = std::shared_ptr<RooFunctor>(
                  w_ggh_->function("ggH_mg_ue_down")->functor(w_ggh_->argSet("ngenjets")));
      }
    }
    else if(scalefactor_file_!="" && !is_embedded_) {
        TFile f(scalefactor_file_.c_str());
        w_ = std::shared_ptr<RooWorkspace>((RooWorkspace*)gDirectory->Get("w"));;
        f.Close();

        if(strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16 || strategy_ == strategy::legacy16 || strategy_ == strategy::cpdecays16){
          fns_["em_qcd_osss_binned_mva"] = std::shared_ptr<RooFunctor>(
            w_->function("em_qcd_osss_binned_mva")->functor(w_->argSet("dR,njets,e_pt,m_pt,nbjets")));
          fns_["em_qcd_osss_shapedown_binned_mva"] = std::shared_ptr<RooFunctor>(
            w_->function("em_qcd_osss_shapedown_binned_mva")->functor(w_->argSet("dR,njets,e_pt,m_pt,nbjets")));
          fns_["em_qcd_osss_shapeup_binned_mva"] = std::shared_ptr<RooFunctor>(
            w_->function("em_qcd_osss_shapeup_binned_mva")->functor(w_->argSet("dR,njets,e_pt,m_pt,nbjets")));
          fns_["em_qcd_osss_ratedown_binned_mva"] = std::shared_ptr<RooFunctor>(
            w_->function("em_qcd_osss_ratedown_binned_mva")->functor(w_->argSet("dR,njets,e_pt,m_pt,nbjets")));
          fns_["em_qcd_osss_rateup_binned_mva"] = std::shared_ptr<RooFunctor>(
            w_->function("em_qcd_osss_rateup_binned_mva")->functor(w_->argSet("dR,njets,e_pt,m_pt,nbjets")));
          fns_["em_qcd_osss_binned_mva_nbjets_up"] = std::shared_ptr<RooFunctor>(
            w_->function("em_qcd_osss_binned_mva_nbjets_up")->functor(w_->argSet("dR,njets,e_pt,m_pt,nbjets")));
          fns_["em_qcd_osss_binned_mva_nbjets_down"] = std::shared_ptr<RooFunctor>(
            w_->function("em_qcd_osss_binned_mva_nbjets_down")->functor(w_->argSet("dR,njets,e_pt,m_pt,nbjets")));
          fns_["em_qcd_extrap_up"] = std::shared_ptr<RooFunctor>(
            w_->function("em_qcd_extrap_up")->functor(w_->argSet("dR,njets,e_pt,m_pt")));
          fns_["em_qcd_extrap_down"] = std::shared_ptr<RooFunctor>(
            w_->function("em_qcd_extrap_down")->functor(w_->argSet("dR,njets,e_pt,m_pt")));
          fns_["em_qcd_osss_binned_bothaiso"] = std::shared_ptr<RooFunctor>(
            w_->function("em_qcd_osss_binned_bothaiso")->functor(w_->argSet("dR,njets,e_pt,m_pt")));
        }
        if(mc_==mc::mc2017 || mc_ == mc::mc2018) {
          fns_["em_qcd_osss_binned"] = std::shared_ptr<RooFunctor>(
            w_->function("em_qcd_osss_binned")->functor(w_->argSet("dR,njets,e_pt,m_pt,iso")));
          fns_["em_qcd_osss_shapedown_binned"] = std::shared_ptr<RooFunctor>(
            w_->function("em_qcd_osss_shapedown_binned")->functor(w_->argSet("dR,njets,e_pt,m_pt,iso")));
          fns_["em_qcd_osss_shapeup_binned"] = std::shared_ptr<RooFunctor>(
            w_->function("em_qcd_osss_shapeup_binned")->functor(w_->argSet("dR,njets,e_pt,m_pt,iso")));
          fns_["em_qcd_osss_ratedown_binned"] = std::shared_ptr<RooFunctor>(
            w_->function("em_qcd_osss_ratedown_binned")->functor(w_->argSet("dR,njets,e_pt,m_pt,iso")));
          fns_["em_qcd_osss_rateup_binned"] = std::shared_ptr<RooFunctor>(
            w_->function("em_qcd_osss_rateup_binned")->functor(w_->argSet("dR,njets,e_pt,m_pt,iso")));
          fns_["em_qcd_extrap_up"] = std::shared_ptr<RooFunctor>(
            w_->function("em_qcd_extrap_up")->functor(w_->argSet("dR,njets,e_pt,m_pt,iso")));
          fns_["em_qcd_extrap_down"] = std::shared_ptr<RooFunctor>(
            w_->function("em_qcd_extrap_down")->functor(w_->argSet("dR,njets,e_pt,m_pt,iso")));
        }

            
        if(do_trg_weights_ || do_idiso_weights_) {
          if(mc_==mc::mc2017 || mc_ == mc::mc2018){
              fns_["m_trg_binned_mc"] = std::shared_ptr<RooFunctor>(
                 w_->function("m_trg_binned_mc")->functor(w_->argSet("m_pt,m_eta,m_iso")));    
              fns_["m_trg_binned_data"] = std::shared_ptr<RooFunctor>(
                 w_->function("m_trg_binned_data")->functor(w_->argSet("m_pt,m_eta,m_iso")));
              fns_["m_trg_binned_ratio"] = std::shared_ptr<RooFunctor>(
                 w_->function("m_trg_binned_ratio")->functor(w_->argSet("m_pt,m_eta,m_iso")));
              fns_["m_trg20_data"] = std::shared_ptr<RooFunctor>(
                 w_->function("m_trg20_data")->functor(w_->argSet("m_pt,m_eta")));
              fns_["m_trg20_mc"] = std::shared_ptr<RooFunctor>(
                 w_->function("m_trg20_mc")->functor(w_->argSet("m_pt,m_eta")));
              fns_["m_idiso_binned_ratio"] = std::shared_ptr<RooFunctor>(
                 w_->function("m_idiso_binned_ratio")->functor(w_->argSet("m_pt,m_eta,m_iso")));
              if (mc_ == mc::mc2018) {
                fns_["m_idiso_desy_ratio"] = std::shared_ptr<RooFunctor>(
                   w_->function("m_idiso_desy_ratio")->functor(w_->argSet("m_pt,m_eta")));
                fns_["m_trgIsoMu24orIsoMu27_desy_data"] = std::shared_ptr<RooFunctor>(
                   w_->function("m_trgIsoMu24orIsoMu27_desy_data")->functor(w_->argSet("m_pt,m_eta")));
                fns_["m_trgIsoMu24orIsoMu27_desy_mc"] = std::shared_ptr<RooFunctor>(
                   w_->function("m_trgIsoMu24orIsoMu27_desy_mc")->functor(w_->argSet("m_pt,m_eta")));

                fns_["m_trgIsoMu20Xtau_desy_data"] = std::shared_ptr<RooFunctor>(
                   w_->function("m_trgIsoMu20Xtau_desy_data")->functor(w_->argSet("m_pt,m_eta")));
                fns_["m_trgIsoMu20Xtau_desy_mc"] = std::shared_ptr<RooFunctor>(
                   w_->function("m_trgIsoMu20Xtau_desy_mc")->functor(w_->argSet("m_pt,m_eta")));
              }
              fns_["m_iso_binned_ratio"] = std::shared_ptr<RooFunctor>(
                 w_->function("m_iso_binned_ratio")->functor(w_->argSet("m_pt,m_eta,m_iso")));
              fns_["m_looseiso_binned_ratio"] = std::shared_ptr<RooFunctor>(
                 w_->function("m_looseiso_binned_ratio")->functor(w_->argSet("m_pt,m_eta,m_iso")));
              fns_["m_id_ratio"] = std::shared_ptr<RooFunctor>(
                 w_->function("m_id_ratio")->functor(w_->argSet("m_pt,m_eta")));
              fns_["e_trg_binned_mc"] = std::shared_ptr<RooFunctor>(
                 w_->function("e_trg_binned_mc")->functor(w_->argSet("e_pt,e_eta,e_iso")));    
              fns_["e_trg_binned_data"] = std::shared_ptr<RooFunctor>(
                 w_->function("e_trg_binned_data")->functor(w_->argSet("e_pt,e_eta,e_iso")));
              fns_["e_trg_binned_ratio"] = std::shared_ptr<RooFunctor>(
                 w_->function("e_trg_binned_ratio")->functor(w_->argSet("e_pt,e_eta,e_iso")));
              fns_["e_trg24_data"] = std::shared_ptr<RooFunctor>(
                w_->function("e_trg24_fromDoubleE_data")->functor(w_->argSet("e_pt,e_eta")));
              fns_["e_trg24_mc"] = std::shared_ptr<RooFunctor>(
               w_->function("e_trg24_fromDoubleE_mc")->functor(w_->argSet("e_pt,e_eta")));
              //fns_["e_trg24_data"] = std::shared_ptr<RooFunctor>(
              //  w_->function("e_trg24_data")->functor(w_->argSet("e_pt,e_eta")));
              //fns_["e_trg24_mc"] = std::shared_ptr<RooFunctor>(
              //   w_->function("e_trg24_mc")->functor(w_->argSet("e_pt,e_eta")));
              if (mc_ == mc::mc2018) {
                fns_["e_idiso_desy_ratio"] = std::shared_ptr<RooFunctor>(
                   w_->function("e_idiso_desy_ratio")->functor(w_->argSet("e_pt,e_eta")));
                fns_["e_trgEle32orEle35_desy_data"] = std::shared_ptr<RooFunctor>(
                   w_->function("e_trgEle32orEle35_desy_data")->functor(w_->argSet("e_pt,e_eta")));
                fns_["e_trgEle32orEle35_desy_mc"] = std::shared_ptr<RooFunctor>(
                   w_->function("e_trgEle32orEle35_desy_mc")->functor(w_->argSet("e_pt,e_eta")));
                fns_["e_trg_EleTau_Ele24Leg_desy_data"] = std::shared_ptr<RooFunctor>(
                   w_->function("e_trg_EleTau_Ele24Leg_desy_data")->functor(w_->argSet("e_pt,e_eta")));
                fns_["e_trg_EleTau_Ele24Leg_desy_mc"] = std::shared_ptr<RooFunctor>(
                   w_->function("e_trg_EleTau_Ele24Leg_desy_mc")->functor(w_->argSet("e_pt,e_eta")));
              }
              fns_["e_idiso_binned_ratio"] = std::shared_ptr<RooFunctor>(
                 w_->function("e_idiso_binned_ratio")->functor(w_->argSet("e_pt,e_eta,e_iso")));
              fns_["e_iso_binned_ratio"] = std::shared_ptr<RooFunctor>(
                 w_->function("e_iso_binned_ratio")->functor(w_->argSet("e_pt,e_eta,e_iso")));
              fns_["e_id_pog_ratio"] = std::shared_ptr<RooFunctor>(
                 w_->function("e_id_pog_ratio")->functor(w_->argSet("e_pt,e_eta")));
              fns_["e_looseid_pog_ratio"] = std::shared_ptr<RooFunctor>(
                 w_->function("e_looseid_pog_ratio")->functor(w_->argSet("e_pt,e_eta")));
              fns_["e_idiso_pog_ratio"] = std::shared_ptr<RooFunctor>(
                 w_->function("e_idiso_pog_ratio")->functor(w_->argSet("e_pt,e_eta")));
              fns_["e_id_ratio"] = std::shared_ptr<RooFunctor>(
                 w_->function("e_id_ratio")->functor(w_->argSet("e_pt,e_eta")));
              fns_["e_looseidiso_pog_ratio"] = std::shared_ptr<RooFunctor>(
                 w_->function("e_looseidiso_pog_ratio")->functor(w_->argSet("e_pt,e_eta")));
              fns_["t_trg_tight_ditau_data"] = std::shared_ptr<RooFunctor>(
                 w_->function("t_trg_tight_ditau_data")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
              fns_["t_trg_tight_ditau_mc"] = std::shared_ptr<RooFunctor>(
                 w_->function("t_trg_tight_ditau_mc")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
              fns_["t_trg_tight_tt_mcclose"] = std::shared_ptr<RooFunctor>(
                w_->function("t_trg_tight_tt_mcclose")->functor(w_->argSet("t_pt,t_eta")));
              fns_["t_trg_tight_mutau_data"] = std::shared_ptr<RooFunctor>(
                 w_->function("t_trg_tight_mutau_data")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
              fns_["t_trg_tight_mutau_mc"] = std::shared_ptr<RooFunctor>(
                 w_->function("t_trg_tight_mutau_mc")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
              fns_["t_trg_tight_etau_data"] = std::shared_ptr<RooFunctor>(
                 w_->function("t_trg_tight_etau_data")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
              fns_["t_trg_tight_etau_mc"] = std::shared_ptr<RooFunctor>(
                 w_->function("t_trg_tight_etau_mc")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
              //fns_["t_trg_tight_tt_up"] = std::shared_ptr<RooFunctor>(
              //   w_->function("t_trg_tight_tt_up")->functor(w_->argSet("t_pt")));
              //fns_["t_trg_tight_tt_down"] = std::shared_ptr<RooFunctor>(
              //   w_->function("t_trg_tight_tt_down")->functor(w_->argSet("t_pt")));
              //fns_["t_trg_tight_mt_up"] = std::shared_ptr<RooFunctor>(
              //   w_->function("t_trg_tight_mt_up")->functor(w_->argSet("t_pt")));
              //fns_["t_trg_tight_mt_down"] = std::shared_ptr<RooFunctor>(
              //   w_->function("t_trg_tight_mt_down")->functor(w_->argSet("t_pt")));
              //fns_["t_trg_tight_et_up"] = std::shared_ptr<RooFunctor>(
              //   w_->function("t_trg_tight_et_up")->functor(w_->argSet("t_pt")));
              //fns_["t_trg_tight_et_down"] = std::shared_ptr<RooFunctor>(
              //   w_->function("t_trg_tight_et_down")->functor(w_->argSet("t_pt")));
              // et cross trigger
              fns_["e_trg_EleTau_Ele24Leg_desy_data"] = std::shared_ptr<RooFunctor>(
                 w_->function("e_trg_EleTau_Ele24Leg_desy_data")->functor(w_->argSet("e_pt,e_eta")));
              fns_["e_trg_EleTau_Ele24Leg_desy_mc"] = std::shared_ptr<RooFunctor>(
                 w_->function("e_trg_EleTau_Ele24Leg_desy_mc")->functor(w_->argSet("e_pt,e_eta")));
              // em cross trigger
              fns_["e_trg_binned_12_mc"] = std::shared_ptr<RooFunctor>(
                 w_->function("e_trg_binned_12_mc")->functor(w_->argSet("e_pt,e_eta,e_iso")));    
              fns_["e_trg_binned_12_data"] = std::shared_ptr<RooFunctor>(
                 w_->function("e_trg_binned_12_data")->functor(w_->argSet("e_pt,e_eta,e_iso")));
              fns_["e_trg_binned_12_ratio"] = std::shared_ptr<RooFunctor>(
                 w_->function("e_trg_binned_12_ratio")->functor(w_->argSet("e_pt,e_eta,e_iso")));
              fns_["e_trg_binned_23_mc"] = std::shared_ptr<RooFunctor>(
                 w_->function("e_trg_binned_23_mc")->functor(w_->argSet("e_pt,e_eta,e_iso")));    
              fns_["e_trg_binned_23_data"] = std::shared_ptr<RooFunctor>(
                 w_->function("e_trg_binned_23_data")->functor(w_->argSet("e_pt,e_eta,e_iso")));
              fns_["e_trg_binned_23_ratio"] = std::shared_ptr<RooFunctor>(
                 w_->function("e_trg_binned_23_ratio")->functor(w_->argSet("e_pt,e_eta,e_iso")));
              fns_["m_trg_binned_8_mc"] = std::shared_ptr<RooFunctor>(
                 w_->function("m_trg_binned_8_mc")->functor(w_->argSet("m_pt,m_eta,m_iso")));    
              fns_["m_trg_binned_8_data"] = std::shared_ptr<RooFunctor>(
                 w_->function("m_trg_binned_8_data")->functor(w_->argSet("m_pt,m_eta,m_iso")));
              fns_["m_trg_binned_8_ratio"] = std::shared_ptr<RooFunctor>(
                 w_->function("m_trg_binned_8_ratio")->functor(w_->argSet("m_pt,m_eta,m_iso")));
              fns_["m_trg_binned_23_mc"] = std::shared_ptr<RooFunctor>(
                 w_->function("m_trg_binned_23_mc")->functor(w_->argSet("m_pt,m_eta,m_iso")));    
              fns_["m_trg_binned_23_data"] = std::shared_ptr<RooFunctor>(
                 w_->function("m_trg_binned_23_data")->functor(w_->argSet("m_pt,m_eta,m_iso")));
              fns_["m_trg_binned_23_ratio"] = std::shared_ptr<RooFunctor>(
                 w_->function("m_trg_binned_23_ratio")->functor(w_->argSet("m_pt,m_eta,m_iso")));


          } else {
            if (strategy_ != strategy::smsummer16 && strategy_ != strategy::cpsummer16 &&  strategy_ != strategy::legacy16 && strategy_ != strategy::cpdecays16) {
        
            fns_["m_id_ratio"] = std::shared_ptr<RooFunctor>(
                w_->function("m_id_ratio")->functor(w_->argSet("m_pt,m_eta")));
            fns_["m_iso_binned_ratio"] = std::shared_ptr<RooFunctor>(
                w_->function("m_iso_binned_ratio")->functor(w_->argSet("m_pt,m_eta,m_iso")));
            /*fns_["m_trg_binned_data"] = std::shared_ptr<RooFunctor>(
               w_->function("m_trg_binned_data")->functor(w_->argSet("m_pt,m_eta,m_iso")));*/
            if(mc_ != mc::summer16_80X){
              fns_["m_trgOR_binned_data"] = std::shared_ptr<RooFunctor>(
                 w_->function("m_trgOR_binned_data")->functor(w_->argSet("m_pt,m_eta,m_iso")));
            } else{
              fns_["m_trgOR4_binned_ratio"] = std::shared_ptr<RooFunctor>(
                 w_->function("m_trgOR4_binned_ratio")->functor(w_->argSet("m_pt,m_eta,m_iso")));
              fns_["m_trgOR4_binned_data"] = std::shared_ptr<RooFunctor>(
                 w_->function("m_trgOR4_binned_data")->functor(w_->argSet("m_pt,m_eta,m_iso")));
              fns_["m_trgOR4_binned_mc"] = std::shared_ptr<RooFunctor>(
                 w_->function("m_trgOR4_binned_mc")->functor(w_->argSet("m_pt,m_eta,m_iso")));
            }}
            fns_["m_id_ratio"] = std::shared_ptr<RooFunctor>(
                w_->function("m_id_ratio")->functor(w_->argSet("m_pt,m_eta")));
            fns_["m_iso_binned_ratio"] = std::shared_ptr<RooFunctor>(
                w_->function("m_iso_binned_ratio")->functor(w_->argSet("m_pt,m_eta,m_iso")));
            fns_["e_id_ratio"] = std::shared_ptr<RooFunctor>(
                w_->function("e_id_ratio")->functor(w_->argSet("e_pt,e_eta")));
            fns_["e_iso_binned_ratio"] = std::shared_ptr<RooFunctor>(
                w_->function("e_iso_binned_ratio")->functor(w_->argSet("e_pt,e_eta,e_iso")));
            if (strategy_ != strategy::legacy16){
                fns_["m_idiso0p15_desy_ratio"] = std::shared_ptr<RooFunctor>(
                    w_->function("m_idiso0p15_desy_ratio")->functor(w_->argSet("m_pt,m_eta")));
                fns_["m_idiso0p20_desy_ratio"] = std::shared_ptr<RooFunctor>(
                    w_->function("m_idiso0p20_desy_ratio")->functor(w_->argSet("m_pt,m_eta")));
                fns_["m_idiso_aiso0p15to0p3_desy_ratio"] = std::shared_ptr<RooFunctor>(
                    w_->function("m_idiso_aiso0p15to0p3_desy_ratio")->functor(w_->argSet("m_pt,m_eta")));
            }
            
            if(mc_ != mc::summer16_80X){
              fns_["m_trgIsoMu22orTkIsoMu22_desy_data"] = std::shared_ptr<RooFunctor>(
                 w_->function("m_trgIsoMu22orTkIsoMu22_desy_data")->functor(w_->argSet("m_pt,m_eta")));
            } else if(strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16  || strategy_ == strategy::cpdecays16) {
              fns_["m_trgMu22OR_eta2p1_desy_data"] = std::shared_ptr<RooFunctor>(
                 w_->function("m_trgMu22OR_eta2p1_desy_data")->functor(w_->argSet("m_pt,m_eta")));
              fns_["m_trgMu22OR_eta2p1_desy_mc"] = std::shared_ptr<RooFunctor>(
                 w_->function("m_trgMu22OR_eta2p1_desy_mc")->functor(w_->argSet("m_pt,m_eta")));
              fns_["m_trgMu22OR_eta2p1_aiso0p15to0p3_desy_data"] = std::shared_ptr<RooFunctor>(
                 w_->function("m_trgMu22OR_eta2p1_aiso0p15to0p3_desy_data")->functor(w_->argSet("m_pt,m_eta")));
              fns_["m_trgMu22OR_eta2p1_aiso0p15to0p3_desy_mc"] = std::shared_ptr<RooFunctor>(
                 w_->function("m_trgMu22OR_eta2p1_aiso0p15to0p3_desy_mc")->functor(w_->argSet("m_pt,m_eta")));
              fns_["m_trgMu19leg_eta2p1_desy_data"] = std::shared_ptr<RooFunctor>(
               w_->function("m_trgMu19leg_eta2p1_desy_data")->functor(w_->argSet("m_pt,m_eta")));
              fns_["m_trgMu19leg_eta2p1_desy_mc"] = std::shared_ptr<RooFunctor>(
               w_->function("m_trgMu19leg_eta2p1_desy_mc")->functor(w_->argSet("m_pt,m_eta")));
              fns_["m_trgMu19leg_eta2p1_aiso0p15to0p3_desy_data"] = std::shared_ptr<RooFunctor>(
               w_->function("m_trgMu19leg_eta2p1_aiso0p15to0p3_desy_data")->functor(w_->argSet("m_pt,m_eta")));
              fns_["m_trgMu19leg_eta2p1_aiso0p15to0p3_desy_mc"] = std::shared_ptr<RooFunctor>(
               w_->function("m_trgMu19leg_eta2p1_aiso0p15to0p3_desy_mc")->functor(w_->argSet("m_pt,m_eta")));
            } else if (strategy_ == strategy::legacy16){
             fns_["m_trg_binned_data"] = std::shared_ptr<RooFunctor>(
               w_->function("m_trg_binned_data")->functor(w_->argSet("m_pt,m_eta,m_iso")));
             fns_["m_trg_binned_mc"] = std::shared_ptr<RooFunctor>(
               w_->function("m_trg_binned_mc")->functor(w_->argSet("m_pt,m_eta,m_iso")));
             fns_["m_trg_binned_ratio"] = std::shared_ptr<RooFunctor>(
               w_->function("m_trg_binned_ratio")->functor(w_->argSet("m_pt,m_eta,m_iso")));
            }
            else{
              fns_["m_trgIsoMu24orTkIsoMu24_desy_data"] = std::shared_ptr<RooFunctor>(
                 w_->function("m_trgIsoMu24orTkIsoMu24_desy_data")->functor(w_->argSet("m_pt,m_eta")));
            }
            if (strategy_ != strategy::legacy16){
            fns_["m_trgMu8leg_desy_data"] = std::shared_ptr<RooFunctor>(
               w_->function("m_trgMu8leg_desy_data")->functor(w_->argSet("m_pt,m_eta")));
            fns_["m_trgMu23leg_desy_data"] = std::shared_ptr<RooFunctor>(
               w_->function("m_trgMu23leg_desy_data")->functor(w_->argSet("m_pt,m_eta")));
            fns_["m_trgMu19leg_eta2p1_desy_data"] = std::shared_ptr<RooFunctor>(
               w_->function("m_trgMu19leg_eta2p1_desy_data")->functor(w_->argSet("m_pt,m_eta")));
            }
            if(mc_ == mc::summer16_80X){
              if (strategy_ != strategy::legacy16){
                 fns_["m_trgMu8leg_desy_mc"] = std::shared_ptr<RooFunctor>(
                    w_->function("m_trgMu8leg_desy_mc")->functor(w_->argSet("m_pt,m_eta")));
                 fns_["m_trgMu23leg_desy_mc"] = std::shared_ptr<RooFunctor>(
                    w_->function("m_trgMu23leg_desy_mc")->functor(w_->argSet("m_pt,m_eta")));
                 fns_["m_trgMu19leg_eta2p1_desy_mc"] = std::shared_ptr<RooFunctor>(
                    w_->function("m_trgMu19leg_eta2p1_desy_mc")->functor(w_->argSet("m_pt,m_eta")));
                }
           if(strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16 || strategy_ == strategy::cpdecays16){
                fns_["t_fake_TightIso_mt_ratio"] = std::shared_ptr<RooFunctor>(
                    w_->function("t_fake_TightIso_mt_ratio")->functor(w_->argSet("t_pt,t_eta")));
                fns_["t_genuine_TightIso_mt_ratio"] = std::shared_ptr<RooFunctor>(
                    w_->function("t_genuine_TightIso_mt_ratio")->functor(w_->argSet("t_pt,t_eta")));
                fns_["t_fake_TightIso_mt_data"] = std::shared_ptr<RooFunctor>(
                    w_->function("t_fake_TightIso_mt_data")->functor(w_->argSet("t_pt,t_eta")));
                fns_["t_genuine_TightIso_mt_data"] = std::shared_ptr<RooFunctor>(
                    w_->function("t_genuine_TightIso_mt_data")->functor(w_->argSet("t_pt,t_eta")));
                fns_["t_trg_tight_tt_mcclose"] = std::shared_ptr<RooFunctor>(
                    w_->function("t_trg_tight_tt_mcclose")->functor(w_->argSet("t_pt,t_dm")));
              }
            }
            if (strategy_ != strategy::smsummer16 && strategy_ != strategy::cpsummer16 &&  strategy_ != strategy::legacy16  && strategy_ != strategy::cpdecays16) {
            fns_["e_id_ratio"] = std::shared_ptr<RooFunctor>(
                w_->function("e_id_ratio")->functor(w_->argSet("e_pt,e_eta")));
            fns_["e_iso_binned_ratio"] = std::shared_ptr<RooFunctor>(
                w_->function("e_iso_binned_ratio")->functor(w_->argSet("e_pt,e_eta,e_iso")));
            fns_["e_trg_binned_data"] = std::shared_ptr<RooFunctor>(
               w_->function("e_trg_binned_data")->functor(w_->argSet("e_pt,e_eta,e_iso")));
            
            if (mc_ == mc::summer16_80X){
              fns_["e_trg_binned_mc"] = std::shared_ptr<RooFunctor>(
                 w_->function("e_trg_binned_mc")->functor(w_->argSet("e_pt,e_eta,e_iso")));
            }
            
            fns_["e_idiso0p15_desy_ratio"] = std::shared_ptr<RooFunctor>(
               w_->function("e_idiso0p15_desy_ratio")->functor(w_->argSet("e_pt,e_eta")));
            if(mc_ != mc::summer16_80X){
              fns_["e_idiso0p10_desy_ratio"] = std::shared_ptr<RooFunctor>(
                 w_->function("e_idiso0p10_desy_ratio")->functor(w_->argSet("e_pt,e_eta")));
            } else{
              fns_["e_idiso0p10_KITbins_desy_ratio"] = std::shared_ptr<RooFunctor>(
                 w_->function("e_idiso0p10_KITbins_desy_ratio")->functor(w_->argSet("e_pt,e_eta")));
            }
            }
            if(strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16 || strategy_ == strategy::cpdecays16){
              fns_["e_idiso0p1_desy_ratio"] = std::shared_ptr<RooFunctor>(
                 w_->function("e_idiso0p1_desy_ratio")->functor(w_->argSet("e_pt,e_eta")));
              fns_["e_idiso_aiso0p1to0p3_desy_ratio"] = std::shared_ptr<RooFunctor>(
                 w_->function("e_idiso_aiso0p1to0p3_desy_ratio")->functor(w_->argSet("e_pt,e_eta")));
              fns_["e_idiso0p15_desy_ratio"] = std::shared_ptr<RooFunctor>(
                 w_->function("e_idiso0p15_desy_ratio")->functor(w_->argSet("e_pt,e_eta")));
            }
            fns_["e_trgEle25eta2p1WPTight_desy_data"] = std::shared_ptr<RooFunctor>(
               w_->function("e_trgEle25eta2p1WPTight_desy_data")->functor(w_->argSet("e_pt,e_eta")));
            fns_["e_trgEle25eta2p1WPTight_aiso0p1to0p3_desy_data"] = std::shared_ptr<RooFunctor>(
               w_->function("e_trgEle25eta2p1WPTight_aiso0p1to0p3_desy_data")->functor(w_->argSet("e_pt,e_eta")));
            fns_["e_trgEle12leg_desy_data"] = std::shared_ptr<RooFunctor>(
               w_->function("e_trgEle12leg_desy_data")->functor(w_->argSet("e_pt,e_eta")));
            fns_["e_trgEle23leg_desy_data"] = std::shared_ptr<RooFunctor>(
               w_->function("e_trgEle23leg_desy_data")->functor(w_->argSet("e_pt,e_eta")));
            if(mc_ == mc::summer16_80X){
              fns_["e_trgEle23leg_desy_mc"] = std::shared_ptr<RooFunctor>(
                 w_->function("e_trgEle23leg_desy_mc")->functor(w_->argSet("e_pt,e_eta")));
              fns_["e_trgEle12leg_desy_mc"] = std::shared_ptr<RooFunctor>(
                 w_->function("e_trgEle12leg_desy_mc")->functor(w_->argSet("e_pt,e_eta")));
              fns_["e_trgEle25eta2p1WPTight_desy_mc"] = std::shared_ptr<RooFunctor>(
                 w_->function("e_trgEle25eta2p1WPTight_desy_mc")->functor(w_->argSet("e_pt,e_eta")));
              fns_["e_trgEle25eta2p1WPTight_aiso0p1to0p3_desy_mc"] = std::shared_ptr<RooFunctor>(
               w_->function("e_trgEle25eta2p1WPTight_aiso0p1to0p3_desy_mc")->functor(w_->argSet("e_pt,e_eta")));
            }
            if(mc_ == mc::summer16_80X){
              fns_["t_fake_MediumIso_tt_mc"] = std::shared_ptr<RooFunctor>(
                  w_->function("t_fake_MediumIso_tt_mc")->functor(w_->argSet("t_pt,t_dm")));
              fns_["t_genuine_MediumIso_tt_mc"] = std::shared_ptr<RooFunctor>(
                  w_->function("t_genuine_MediumIso_tt_mc")->functor(w_->argSet("t_pt,t_dm")));
              fns_["t_fake_MediumIso_tt_data"] = std::shared_ptr<RooFunctor>(
                  w_->function("t_fake_MediumIso_tt_data")->functor(w_->argSet("t_pt,t_dm")));
              fns_["t_genuine_MediumIso_tt_data"] = std::shared_ptr<RooFunctor>(
                  w_->function("t_genuine_MediumIso_tt_data")->functor(w_->argSet("t_pt,t_dm")));
              fns_["t_genuine_LooseIso_tt_data"] = std::shared_ptr<RooFunctor>(
                  w_->function("t_genuine_LooseIso_tt_data")->functor(w_->argSet("t_pt,t_dm")));
              fns_["t_fake_TightIso_tt_mc"] = std::shared_ptr<RooFunctor>(
                  w_->function("t_fake_TightIso_tt_mc")->functor(w_->argSet("t_pt,t_dm")));
              fns_["t_genuine_TightIso_tt_mc"] = std::shared_ptr<RooFunctor>(
                  w_->function("t_genuine_TightIso_tt_mc")->functor(w_->argSet("t_pt,t_dm")));
              fns_["t_fake_TightIso_tt_data"] = std::shared_ptr<RooFunctor>(
                  w_->function("t_fake_TightIso_tt_data")->functor(w_->argSet("t_pt,t_dm")));
              fns_["t_genuine_TightIso_tt_data"] = std::shared_ptr<RooFunctor>(
                  w_->function("t_genuine_TightIso_tt_data")->functor(w_->argSet("t_pt,t_dm")));
            } else{
              fns_["t_trgLooseIso_data"] = std::shared_ptr<RooFunctor>(
                 w_->function("t_trgLooseIso_data")->functor(w_->argSet("t_pt")));
              fns_["t_trgMediumIso_data"] = std::shared_ptr<RooFunctor>(
                 w_->function("t_trgMediumIso_data")->functor(w_->argSet("t_pt")));
              fns_["t_trgTightIso_data"] = std::shared_ptr<RooFunctor>(
                 w_->function("t_trgTightIso_data")->functor(w_->argSet("t_pt")));
              fns_["t_trgVTightIso_data"] = std::shared_ptr<RooFunctor>(
                 w_->function("t_trgVTightIso_data")->functor(w_->argSet("t_pt")));
              fns_["t_trgLooseIsoSS_data"] = std::shared_ptr<RooFunctor>(
                 w_->function("t_trgLooseIsoSS_data")->functor(w_->argSet("t_pt")));
              fns_["t_trgMediumIsoSS_data"] = std::shared_ptr<RooFunctor>(
                 w_->function("t_trgMediumIsoSS_data")->functor(w_->argSet("t_pt")));
              fns_["t_trgTightIsoSS_data"] = std::shared_ptr<RooFunctor>(
                 w_->function("t_trgTightIsoSS_data")->functor(w_->argSet("t_pt")));
              fns_["t_trgVTightIsoSS_data"] = std::shared_ptr<RooFunctor>(
                 w_->function("t_trgVTightIsoSS_data")->functor(w_->argSet("t_pt")));
            }
          }
          if(do_tau_id_sf_ && strategy_ != strategy::smsummer16 && strategy_ != strategy::cpsummer16 &&  strategy_ != strategy::legacy16 &&  strategy_ != strategy::cpdecays16 && mc_!=mc::mc2017 && mc_ != mc::mc2018){
            fns_["t_iso_mva_m_pt30_sf"] = std::shared_ptr<RooFunctor>(
               w_->function("t_iso_mva_m_pt30_sf")->functor(w_->argSet("t_pt,t_eta,t_dm")));
            fns_["t_iso_mva_t_pt40_eta2p1_sf"] = std::shared_ptr<RooFunctor>(
               w_->function("t_iso_mva_t_pt40_eta2p1_sf")->functor(w_->argSet("t_pt,t_eta,t_dm")));
          } 
          if(do_tracking_eff_) {
            fns_["m_trk_ratio"] = std::shared_ptr<RooFunctor>(
                w_->function("m_trk_ratio")->functor(w_->argSet("m_eta")));
            fns_["e_trk_ratio"] = std::shared_ptr<RooFunctor>(
                w_->function("e_trk_ratio")->functor(w_->argSet("e_pt,e_eta")));  
          }
      }
    }
    else if(embedding_scalefactor_file_!="" && scalefactor_file_!="" && is_embedded_) {
        
        TFile f(scalefactor_file_.c_str());
        w_ = std::shared_ptr<RooWorkspace>((RooWorkspace*)gDirectory->Get("w"));;
        f.Close();

        if(strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16 ||  strategy_ == strategy::legacy16 || strategy_ == strategy::cpdecays16) {
          fns_["em_qcd_osss_binned_mva"] = std::shared_ptr<RooFunctor>(
            w_->function("em_qcd_osss_binned_mva")->functor(w_->argSet("dR,njets,e_pt,m_pt,nbjets")));
          fns_["em_qcd_osss_shapedown_binned_mva"] = std::shared_ptr<RooFunctor>(
            w_->function("em_qcd_osss_shapedown_binned_mva")->functor(w_->argSet("dR,njets,e_pt,m_pt,nbjets")));
          fns_["em_qcd_osss_shapeup_binned_mva"] = std::shared_ptr<RooFunctor>(
            w_->function("em_qcd_osss_shapeup_binned_mva")->functor(w_->argSet("dR,njets,e_pt,m_pt,nbjets")));
          fns_["em_qcd_osss_ratedown_binned_mva"] = std::shared_ptr<RooFunctor>(
            w_->function("em_qcd_osss_ratedown_binned_mva")->functor(w_->argSet("dR,njets,e_pt,m_pt,nbjets")));
          fns_["em_qcd_osss_rateup_binned_mva"] = std::shared_ptr<RooFunctor>(
            w_->function("em_qcd_osss_rateup_binned_mva")->functor(w_->argSet("dR,njets,e_pt,m_pt,nbjets")));
          fns_["em_qcd_osss_binned_mva_nbjets_up"] = std::shared_ptr<RooFunctor>(
            w_->function("em_qcd_osss_binned_mva_nbjets_up")->functor(w_->argSet("dR,njets,e_pt,m_pt,nbjets")));
          fns_["em_qcd_osss_binned_mva_nbjets_down"] = std::shared_ptr<RooFunctor>(
            w_->function("em_qcd_osss_binned_mva_nbjets_down")->functor(w_->argSet("dR,njets,e_pt,m_pt,nbjets")));
          fns_["em_qcd_extrap_up"] = std::shared_ptr<RooFunctor>(
            w_->function("em_qcd_extrap_up")->functor(w_->argSet("dR,njets,e_pt,m_pt"))); 
          fns_["em_qcd_extrap_down"] = std::shared_ptr<RooFunctor>(
            w_->function("em_qcd_extrap_down")->functor(w_->argSet("dR,njets,e_pt,m_pt")));
          fns_["em_qcd_osss_binned_bothaiso"] = std::shared_ptr<RooFunctor>(
            w_->function("em_qcd_osss_binned_bothaiso")->functor(w_->argSet("dR,njets,e_pt,m_pt")));
        }
        if(mc_==mc::mc2017 || mc_ == mc::mc2018) {
          fns_["em_qcd_osss_binned"] = std::shared_ptr<RooFunctor>(
            w_->function("em_qcd_osss_binned")->functor(w_->argSet("dR,njets,e_pt,m_pt,iso")));
          fns_["em_qcd_osss_shapedown_binned"] = std::shared_ptr<RooFunctor>(
            w_->function("em_qcd_osss_shapedown_binned")->functor(w_->argSet("dR,njets,e_pt,m_pt,iso")));
          fns_["em_qcd_osss_shapeup_binned"] = std::shared_ptr<RooFunctor>(
            w_->function("em_qcd_osss_shapeup_binned")->functor(w_->argSet("dR,njets,e_pt,m_pt,iso")));
          fns_["em_qcd_osss_ratedown_binned"] = std::shared_ptr<RooFunctor>(
            w_->function("em_qcd_osss_ratedown_binned")->functor(w_->argSet("dR,njets,e_pt,m_pt,iso")));
          fns_["em_qcd_osss_rateup_binned"] = std::shared_ptr<RooFunctor>(
            w_->function("em_qcd_osss_rateup_binned")->functor(w_->argSet("dR,njets,e_pt,m_pt,iso")));
          fns_["em_qcd_extrap_up"] = std::shared_ptr<RooFunctor>(
            w_->function("em_qcd_extrap_up")->functor(w_->argSet("dR,njets,e_pt,m_pt,iso")));
          fns_["em_qcd_extrap_down"] = std::shared_ptr<RooFunctor>(
            w_->function("em_qcd_extrap_down")->functor(w_->argSet("dR,njets,e_pt,m_pt,iso")));
        }

        
        if(do_tracking_eff_) {
          fns_["m_trk_ratio"] = std::shared_ptr<RooFunctor>(
              w_->function("m_trk_ratio")->functor(w_->argSet("m_eta")));
          fns_["e_trk_ratio"] = std::shared_ptr<RooFunctor>(
              w_->function("e_trk_ratio")->functor(w_->argSet("e_pt,e_eta")));  
        }
        
        if(!(mc_==mc::mc2017 || mc_ == mc::mc2018)) { 

            fns_["t_fake_TightIso_mt_data"] = std::shared_ptr<RooFunctor>(
                w_->function("t_fake_TightIso_mt_data")->functor(w_->argSet("t_pt,t_eta")));
            fns_["t_genuine_TightIso_mt_data"] = std::shared_ptr<RooFunctor>(
                w_->function("t_genuine_TightIso_mt_data")->functor(w_->argSet("t_pt,t_eta")));
            fns_["t_fake_TightIso_tt_data"] = std::shared_ptr<RooFunctor>(
                w_->function("t_fake_TightIso_tt_data")->functor(w_->argSet("t_pt,t_dm")));
            fns_["t_genuine_TightIso_tt_data"] = std::shared_ptr<RooFunctor>(
                w_->function("t_genuine_TightIso_tt_data")->functor(w_->argSet("t_pt,t_dm")));
            fns_["t_TightIso_tt_embed"] = std::shared_ptr<RooFunctor>(
                w_->function("t_TightIso_tt_embed")->functor(w_->argSet("t_pt,t_dm")));
            fns_["t_TightIso_mt_embed"] = std::shared_ptr<RooFunctor>(
                w_->function("t_TightIso_mt_embed")->functor(w_->argSet("t_pt,t_eta")));
            fns_["m_trg8_binned_ic_data"] = std::shared_ptr<RooFunctor>(
                  w_->function("m_trg8_binned_ic_data")->functor(w_->argSet("m_pt,m_eta,m_iso")));
            fns_["m_trg8_binned_ic_embed"] = std::shared_ptr<RooFunctor>(
                  w_->function("m_trg8_binned_ic_embed")->functor(w_->argSet("m_pt,m_eta,m_iso")));
            fns_["m_trg23_binned_ic_data"] = std::shared_ptr<RooFunctor>(
                  w_->function("m_trg23_binned_ic_data")->functor(w_->argSet("m_pt,m_eta,m_iso")));
            fns_["m_trg23_binned_ic_embed"] = std::shared_ptr<RooFunctor>(
                  w_->function("m_trg23_binned_ic_embed")->functor(w_->argSet("m_pt,m_eta,m_iso")));
            fns_["m_trg19_binned_ic_data"] = std::shared_ptr<RooFunctor>(
                  w_->function("m_trg19_binned_ic_data")->functor(w_->argSet("m_pt,m_eta,m_iso")));
            fns_["m_trg19_binned_ic_embed"] = std::shared_ptr<RooFunctor>(
                  w_->function("m_trg19_binned_ic_embed")->functor(w_->argSet("m_pt,m_eta,m_iso")));
            fns_["e_trg12_binned_ic_data"] = std::shared_ptr<RooFunctor>(
                  w_->function("e_trg12_binned_ic_data")->functor(w_->argSet("e_pt,e_eta,e_iso")));
            fns_["e_trg12_binned_ic_embed"] = std::shared_ptr<RooFunctor>(
                  w_->function("e_trg12_binned_ic_embed")->functor(w_->argSet("e_pt,e_eta,e_iso")));
            fns_["e_trg23_binned_ic_data"] = std::shared_ptr<RooFunctor>(
                  w_->function("e_trg23_binned_ic_data")->functor(w_->argSet("e_pt,e_eta,e_iso")));
            fns_["e_trg23_binned_ic_embed"] = std::shared_ptr<RooFunctor>(
                  w_->function("e_trg23_binned_ic_embed")->functor(w_->argSet("e_pt,e_eta,e_iso")));

            fns_["m_sel_trg_ratio"] = std::shared_ptr<RooFunctor>(
                 w_->function("m_sel_trg_ratio")->functor(w_->argSet("gt1_pt,gt1_eta,gt2_pt,gt2_eta")));

            fns_["m_looseiso_embed_ratio"] = std::shared_ptr<RooFunctor>(
              w_->function("m_looseiso_embed_ratio")->functor(w_->argSet("m_pt,m_eta")));


            TFile fembed(embedding_scalefactor_file_.c_str());
            wembed_ = std::shared_ptr<RooWorkspace>((RooWorkspace*)gDirectory->Get("w"));;
            fembed.Close();

            fns_["m_id_ratio"] = std::shared_ptr<RooFunctor>(
              wembed_->function("m_id_ratio")->functor(wembed_->argSet("m_pt,m_eta"))); 
            fns_["m_iso_binned_ratio"] = std::shared_ptr<RooFunctor>(
              wembed_->function("m_iso_binned_ratio")->functor(wembed_->argSet("m_pt,m_eta,m_iso")));
            fns_["m_trg_binned_data"] = std::shared_ptr<RooFunctor>(
             wembed_->function("m_trg_binned_data")->functor(wembed_->argSet("m_pt,m_eta,m_iso")));
            fns_["m_trg_binned_mc"] = std::shared_ptr<RooFunctor>(
              wembed_->function("m_trg_binned_mc")->functor(wembed_->argSet("m_pt,m_eta,m_iso")));
            //fns_["m_looseiso_ratio"] = std::shared_ptr<RooFunctor>(
            //  wembed_->function("m_looseiso_ratio")->functor(wembed_->argSet("m_pt,m_eta")));
            fns_["e_id_ratio"] = std::shared_ptr<RooFunctor>(
                 wembed_->function("e_id_ratio")->functor(wembed_->argSet("e_pt,e_eta")));
            fns_["e_iso_binned_ratio"] = std::shared_ptr<RooFunctor>(
                 wembed_->function("e_iso_binned_ratio")->functor(wembed_->argSet("e_pt,e_eta,e_iso")));
            fns_["e_looseiso_ratio"] = std::shared_ptr<RooFunctor>(
                 wembed_->function("e_looseiso_ratio")->functor(wembed_->argSet("e_pt,e_eta")));
            fns_["e_trg_binned_data"] = std::shared_ptr<RooFunctor>(
                 wembed_->function("e_trg_binned_data")->functor(wembed_->argSet("e_pt,e_eta,e_iso")));
            fns_["e_trg_binned_mc"] = std::shared_ptr<RooFunctor>(
                 wembed_->function("e_trg_binned_mc")->functor(wembed_->argSet("e_pt,e_eta,e_iso")));
            fns_["m_sel_idEmb_ratio"] = std::shared_ptr<RooFunctor>(
                 wembed_->function("m_sel_idEmb_ratio")->functor(wembed_->argSet("gt_eta,gt_pt")));

         } else {
           fns_["m_trg_binned_embed"] = std::shared_ptr<RooFunctor>(
              w_->function("m_trg_binned_embed")->functor(w_->argSet("m_pt,m_eta,m_iso")));
           fns_["m_trg_binned_data"] = std::shared_ptr<RooFunctor>(
              w_->function("m_trg_binned_data")->functor(w_->argSet("m_pt,m_eta,m_iso")));
           fns_["m_trg_binned_embed_ratio"] = std::shared_ptr<RooFunctor>(
              w_->function("m_trg_binned_embed_ratio")->functor(w_->argSet("m_pt,m_eta,m_iso")));
           fns_["m_trg20_data"] = std::shared_ptr<RooFunctor>(
              w_->function("m_trg20_data")->functor(w_->argSet("m_pt,m_eta")));
           fns_["m_trg20_embed"] = std::shared_ptr<RooFunctor>(
              w_->function("m_trg20_embed")->functor(w_->argSet("m_pt,m_eta")));
           fns_["m_idiso_binned_embed_ratio"] = std::shared_ptr<RooFunctor>(
              w_->function("m_idiso_binned_embed_ratio")->functor(w_->argSet("m_pt,m_eta,m_iso")));
           fns_["m_iso_binned_embed_ratio"] = std::shared_ptr<RooFunctor>(
              w_->function("m_iso_binned_embed_ratio")->functor(w_->argSet("m_pt,m_eta,m_iso")));
           fns_["m_looseiso_binned_embed_ratio"] = std::shared_ptr<RooFunctor>(
                 w_->function("m_looseiso_binned_embed_ratio")->functor(w_->argSet("m_pt,m_eta,m_iso")));
           fns_["m_id_embed_ratio"] = std::shared_ptr<RooFunctor>(
              w_->function("m_id_embed_ratio")->functor(w_->argSet("m_pt,m_eta")));
           fns_["e_trg_binned_embed"] = std::shared_ptr<RooFunctor>(
              w_->function("e_trg_binned_embed")->functor(w_->argSet("e_pt,e_eta,e_iso")));
           fns_["e_trg_binned_data"] = std::shared_ptr<RooFunctor>(
              w_->function("e_trg_binned_data")->functor(w_->argSet("e_pt,e_eta,e_iso")));
           fns_["e_trg_binned_embed_ratio"] = std::shared_ptr<RooFunctor>(
              w_->function("e_trg_binned_embed_ratio")->functor(w_->argSet("e_pt,e_eta,e_iso")));
           fns_["e_trg24_data"] = std::shared_ptr<RooFunctor>(
              w_->function("e_trg24_fromDoubleE_data")->functor(w_->argSet("e_pt,e_eta")));
           fns_["e_trg24_embed"] = std::shared_ptr<RooFunctor>(
              w_->function("e_trg24_fromDoubleE_embed")->functor(w_->argSet("e_pt,e_eta"))); 
           //fns_["e_trg24_data"] = std::shared_ptr<RooFunctor>(
           //   w_->function("e_trg24_data")->functor(w_->argSet("e_pt,e_eta")));
           //fns_["e_trg24_embed"] = std::shared_ptr<RooFunctor>(
           //   w_->function("e_trg24_embed")->functor(w_->argSet("e_pt,e_eta")));
           fns_["e_idiso_binned_embed_ratio"] = std::shared_ptr<RooFunctor>(
              w_->function("e_idiso_binned_embed_ratio")->functor(w_->argSet("e_pt,e_eta,e_iso")));
           fns_["e_iso_binned_embed_ratio"] = std::shared_ptr<RooFunctor>(
              w_->function("e_iso_binned_embed_ratio")->functor(w_->argSet("e_pt,e_eta,e_iso")));
           fns_["e_id_embed_ratio"] = std::shared_ptr<RooFunctor>(
              w_->function("e_id_embed_ratio")->functor(w_->argSet("e_pt,e_eta")));
           fns_["e_trg_EleTau_Ele24Leg_desy_data"] = std::shared_ptr<RooFunctor>(
              w_->function("e_trg_EleTau_Ele24Leg_desy_data")->functor(w_->argSet("e_pt,e_eta")));
           fns_["t_trg_tight_ditau_data"] = std::shared_ptr<RooFunctor>(
              w_->function("t_trg_tight_ditau_data")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
           fns_["t_trg_tight_ditau_mc"] = std::shared_ptr<RooFunctor>(
              w_->function("t_trg_tight_ditau_mc")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
           fns_["t_trg_tight_ditau_embed"] = std::shared_ptr<RooFunctor>(
              w_->function("t_trg_tight_ditau_embed")->functor(w_->argSet("t_pt,t_eta,t_dm")));
           fns_["t_trg_tight_etau_embed"] = std::shared_ptr<RooFunctor>(
              w_->function("t_trg_tight_etau_embed")->functor(w_->argSet("t_pt,t_eta,t_dm")));
           fns_["t_trg_tight_mutau_embed"] = std::shared_ptr<RooFunctor>(
              w_->function("t_trg_tight_mutau_embed")->functor(w_->argSet("t_pt,t_eta,t_dm")));
           fns_["t_trg_tight_mutau_data"] = std::shared_ptr<RooFunctor>(
              w_->function("t_trg_tight_mutau_data")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
           //fns_["t_trg_tight_tt_up"] = std::shared_ptr<RooFunctor>(
           //   w_->function("t_trg_tight_tt_up")->functor(w_->argSet("t_pt")));
           //fns_["t_trg_tight_tt_down"] = std::shared_ptr<RooFunctor>(
           //   w_->function("t_trg_tight_tt_down")->functor(w_->argSet("t_pt")));
           //fns_["t_trg_tight_mt_up"] = std::shared_ptr<RooFunctor>(
           //   w_->function("t_trg_tight_mt_up")->functor(w_->argSet("t_pt")));
           //fns_["t_trg_tight_mt_down"] = std::shared_ptr<RooFunctor>(
           //   w_->function("t_trg_tight_mt_down")->functor(w_->argSet("t_pt")));
           //fns_["t_trg_tight_et_up"] = std::shared_ptr<RooFunctor>(
           //   w_->function("t_trg_tight_et_up")->functor(w_->argSet("t_pt")));
           //fns_["t_trg_tight_et_down"] = std::shared_ptr<RooFunctor>(
           //   w_->function("t_trg_tight_et_down")->functor(w_->argSet("t_pt")));
           fns_["t_trg_tight_etau_data"] = std::shared_ptr<RooFunctor>(
              w_->function("t_trg_tight_etau_data")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
           fns_["t_trg_tight_etau_mc"] = std::shared_ptr<RooFunctor>(
              w_->function("t_trg_tight_etau_mc")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
           fns_["t_trg_nonclosure"] = std::shared_ptr<RooFunctor>(
              w_->function("t_trg_nonclosure")->functor(w_->argSet("t_pt_1,t_pt_2")));
           fns_["m_sel_idEmb_ratio"] = std::shared_ptr<RooFunctor>(
               w_->function("m_sel_idEmb_ratio")->functor(w_->argSet("gt_eta,gt_pt")));
           fns_["m_sel_trg_ratio"] = std::shared_ptr<RooFunctor>(
               w_->function("m_sel_trg_ratio")->functor(w_->argSet("gt1_pt,gt1_eta,gt2_pt,gt2_eta")));

           // em cross trigger
           fns_["e_trg_binned_12_embed"] = std::shared_ptr<RooFunctor>(
              w_->function("e_trg_binned_12_embed")->functor(w_->argSet("e_pt,e_eta,e_iso")));
           fns_["e_trg_binned_12_data"] = std::shared_ptr<RooFunctor>(
              w_->function("e_trg_binned_12_data")->functor(w_->argSet("e_pt,e_eta,e_iso")));
           fns_["e_trg_binned_23_embed"] = std::shared_ptr<RooFunctor>(
              w_->function("e_trg_binned_23_embed")->functor(w_->argSet("e_pt,e_eta,e_iso")));
           fns_["e_trg_binned_23_data"] = std::shared_ptr<RooFunctor>(
              w_->function("e_trg_binned_23_data")->functor(w_->argSet("e_pt,e_eta,e_iso")));
           fns_["m_trg_binned_8_embed"] = std::shared_ptr<RooFunctor>(
              w_->function("m_trg_binned_8_embed")->functor(w_->argSet("m_pt,m_eta,m_iso")));
           fns_["m_trg_binned_8_data"] = std::shared_ptr<RooFunctor>(
              w_->function("m_trg_binned_8_data")->functor(w_->argSet("m_pt,m_eta,m_iso")));
           fns_["m_trg_binned_23_embed"] = std::shared_ptr<RooFunctor>(
              w_->function("m_trg_binned_23_embed")->functor(w_->argSet("m_pt,m_eta,m_iso")));
           fns_["m_trg_binned_23_data"] = std::shared_ptr<RooFunctor>(
              w_->function("m_trg_binned_23_data")->functor(w_->argSet("m_pt,m_eta,m_iso")));

        }

    }
    if(mssm_higgspt_file_!="" && do_mssm_higgspt_){
      TFile f(mssm_higgspt_file_.c_str());
      mssm_w_ = std::shared_ptr<RooWorkspace>((RooWorkspace*)gDirectory->Get("w"));
      f.Close();
      std::string mass_str = mssm_mass_;
      fns_["h_t_ratio"] = std::shared_ptr<RooFunctor>(mssm_w_->function(("h_"+mass_str+"_t_ratio").c_str())->functor(mssm_w_->argSet("h_pt")));        
      fns_["h_b_ratio"] = std::shared_ptr<RooFunctor>(mssm_w_->function(("h_"+mass_str+"_b_ratio").c_str())->functor(mssm_w_->argSet("h_pt")));
      fns_["h_i_ratio"] = std::shared_ptr<RooFunctor>(mssm_w_->function(("h_"+mass_str+"_i_ratio").c_str())->functor(mssm_w_->argSet("h_pt")));
      fns_["H_t_ratio"] = std::shared_ptr<RooFunctor>(mssm_w_->function(("H_"+mass_str+"_t_ratio").c_str())->functor(mssm_w_->argSet("h_pt")));
      fns_["H_b_ratio"] = std::shared_ptr<RooFunctor>(mssm_w_->function(("H_"+mass_str+"_b_ratio").c_str())->functor(mssm_w_->argSet("h_pt")));
      fns_["H_i_ratio"] = std::shared_ptr<RooFunctor>(mssm_w_->function(("H_"+mass_str+"_i_ratio").c_str())->functor(mssm_w_->argSet("h_pt")));
      fns_["A_t_ratio"] = std::shared_ptr<RooFunctor>(mssm_w_->function(("A_"+mass_str+"_t_ratio").c_str())->functor(mssm_w_->argSet("h_pt")));
      fns_["A_b_ratio"] = std::shared_ptr<RooFunctor>(mssm_w_->function(("A_"+mass_str+"_b_ratio").c_str())->functor(mssm_w_->argSet("h_pt")));
      fns_["A_i_ratio"] = std::shared_ptr<RooFunctor>(mssm_w_->function(("A_"+mass_str+"_i_ratio").c_str())->functor(mssm_w_->argSet("h_pt")));

    }
    if (do_zpt_weight_){
      if (mc_ == mc::summer16_80X && strategy_ == strategy::mssmsummer16){  
        fns_["zpt_weight_nom"] = std::shared_ptr<RooFunctor>( 
                w_->function("zpt_weight_nom")->functor(w_->argSet("z_gen_mass,z_gen_pt"))); 
        fns_["zpt_weight_esup"] = std::shared_ptr<RooFunctor>(
                w_->function("zpt_weight_esup")->functor(w_->argSet("z_gen_mass,z_gen_pt"))); 
        fns_["zpt_weight_esdown"] = std::shared_ptr<RooFunctor>(
                w_->function("zpt_weight_esdown")->functor(w_->argSet("z_gen_mass,z_gen_pt"))); 
        fns_["zpt_weight_ttup"] = std::shared_ptr<RooFunctor>( 
                w_->function("zpt_weight_ttup")->functor(w_->argSet("z_gen_mass,z_gen_pt"))); 
        fns_["zpt_weight_ttdown"] = std::shared_ptr<RooFunctor>( 
                w_->function("zpt_weight_ttdown")->functor(w_->argSet("z_gen_mass,z_gen_pt"))); 
        fns_["zpt_weight_statpt0up"] = std::shared_ptr<RooFunctor>( 
                w_->function("zpt_weight_statpt0up")->functor(w_->argSet("z_gen_mass,z_gen_pt"))); 
        fns_["zpt_weight_statpt0down"] = std::shared_ptr<RooFunctor>(
                w_->function("zpt_weight_statpt0down")->functor(w_->argSet("z_gen_mass,z_gen_pt"))); 
        fns_["zpt_weight_statpt40up"] = std::shared_ptr<RooFunctor>(
                w_->function("zpt_weight_statpt40up")->functor(w_->argSet("z_gen_mass,z_gen_pt"))); 
        fns_["zpt_weight_statpt40down"] = std::shared_ptr<RooFunctor>(
                w_->function("zpt_weight_statpt40down")->functor(w_->argSet("z_gen_mass,z_gen_pt"))); 
        fns_["zpt_weight_statpt80up"] = std::shared_ptr<RooFunctor>(
                w_->function("zpt_weight_statpt80up")->functor(w_->argSet("z_gen_mass,z_gen_pt"))); 
        fns_["zpt_weight_statpt80down"] = std::shared_ptr<RooFunctor>(
                w_->function("zpt_weight_statpt80down")->functor(w_->argSet("z_gen_mass,z_gen_pt"))); 
      }
      
    }

    return 0;
  }

  int HTTWeights::Execute(TreeEvent *event) {

    std::vector<CompositeCandidate *> const& dilepton = event->GetPtrVec<CompositeCandidate>(ditau_label_);
    //std::vector<CompositeCandidate *> dilepton;

    double weight = 1.0;
    EventInfo * eventInfo = event->GetPtr<EventInfo>("eventInfo");

    if (ggh_mass_ != "") {
      std::vector<GenParticle *> const& parts = event->GetPtrVec<GenParticle>("genParticles");
      GenParticle const* higgs = NULL;
      for (unsigned i = 0; i < parts.size(); ++i) {
        if (parts[i]->status() == 3 && parts[i]->pdgid() == 25) {
          higgs = parts[i];
          break;
        }
      }
      if (!higgs) {
        std::cout << "Higgs not found!" << std::endl;
        throw;
      }
      double h_pt = higgs->pt();
      double pt_weight = 1.0;
      int fbin = ggh_hist_->FindBin(h_pt);
      if (fbin > 0 && fbin <= ggh_hist_->GetNbinsX()) {
        pt_weight =  ggh_hist_->GetBinContent(fbin);
        //std::cout << "pt: " << h_pt << "\tweight: " <<  pt_weight << std::endl;
      }
      eventInfo->set_weight("ggh", pt_weight);
      if (mc_ == mc::summer12_53X || mc_ == mc::fall11_42X) {
        double weight_up   = ggh_hist_up_->GetBinContent(fbin)   / pt_weight;
        double weight_down = ggh_hist_down_->GetBinContent(fbin) / pt_weight;
        event->Add("wt_ggh_pt_up", weight_up);
        event->Add("wt_ggh_pt_down", weight_down);
      }
    }
    if(is_embedded_ && (era_==era::data_2016 || era_==era::data_2017 || era_==era::data_2018)){
      // For 2016 embedded samples need an extra weight to account for computing efficiency
      //double wt_stitching = 1.0;  
      //int run = eventInfo->run();
      //if(channel_==channel::mt){
      //  wt_stitching = (((run >= 272007) && (run < 275657))*(1.0/0.899)+((run >= 275657) && (run < 276315))*(1.0/0.881)+((run >= 276315) && (run < 276831))*(1.0/0.877)+((run >= 276831) && (run < 277772))*(1.0/0.939)+((run >= 277772) && (run < 278820))*(1.0/0.936)+((run >= 278820) && (run < 280919))*(1.0/0.908)+((run >= 280919) && (run < 284045))*(1.0/0.962));  
      //} else if(channel_==channel::et) {
      //  wt_stitching = (((run >= 272007) && (run < 275657))*(1.0/0.902)+((run >= 275657) && (run < 276315))*(1.0/0.910)+((run >= 276315) && (run < 276831))*(1.0/0.945)+((run >= 276831) && (run < 277772))*(1.0/0.945)+((run >= 277772) && (run < 278820))*(1.0/0.915)+((run >= 278820) && (run < 280919))*(1.0/0.903)+((run >= 280919) && (run < 284045))*(1.0/0.933));    
      //} else if(channel_==channel::em) {
      //  wt_stitching = (((run >= 272007) && (run < 275657))*(1.0/0.891)+((run >= 275657) && (run < 276315))*(1.0/0.910)+((run >= 276315) && (run < 276831))*(1.0/0.953)+((run >= 276831) && (run < 277772))*(1.0/0.947)+((run >= 277772) && (run < 278820))*(1.0/0.942)+((run >= 278820) && (run < 280919))*(1.0/0.906)+((run >= 280919) && (run < 284045))*(1.0/0.950));  
      //    
      //} else if(channel_==channel::tt) {
      //  wt_stitching = (((run >= 272007) && (run < 275657))*(1.0/0.897)+((run >= 275657) && (run < 276315))*(1.0/0.908)+((run >= 276315) && (run < 276831))*(1.0/0.950)+((run >= 276831) && (run < 277772))*(1.0/0.861)+((run >= 277772) && (run < 278820))*(1.0/0.941)+((run >= 278820) && (run < 280919))*(1.0/0.908)+((run >= 280919) && (run < 284045))*(1.0/0.949));  
      //} else if(channel_==channel::zmm){
      //  wt_stitching = ((run >= 272007) && (run < 275657))*(1.0/0.902)+((run >= 275657) && (run < 276315))*(1.0/0.910)+((run >= 276315) && (run < 276831))*(1.0/0.954)+((run >= 276831) && (run < 277772))*(1.0/0.946)+((run >= 277772) && (run < 278820))*(1.0/0.942)+((run >= 278820) && (run < 280919))*(1.0/0.855)+((run >= 280919) && (run < 284045))*(1.0/0.876);  
      //}
      //if(eventInfo->weight_defined("wt_embedding")) {
      //  if (eventInfo->weight("wt_embedding") > 1) wt_stitching = 0.0; // have to exclude unphysical events i/e where the generator weight is > 1
      //}
      //if(era_==era::data_2016) eventInfo->set_weight("wt_stitching", wt_stitching);
      double gen_match_undecayed_1_pt = event->Get<double>("gen_match_undecayed_1_pt");
      double gen_match_undecayed_2_pt = event->Get<double>("gen_match_undecayed_2_pt");
      double gen_match_undecayed_1_eta = event->Get<double>("gen_match_undecayed_1_eta");
      double gen_match_undecayed_2_eta = event->Get<double>("gen_match_undecayed_2_eta");
      auto args_1 = std::vector<double>{gen_match_undecayed_1_eta,gen_match_undecayed_1_pt};
      auto args_2 = std::vector<double>{gen_match_undecayed_2_eta,gen_match_undecayed_2_pt};
      auto args_4 = std::vector<double>{gen_match_undecayed_1_pt,gen_match_undecayed_1_eta,gen_match_undecayed_2_pt,gen_match_undecayed_2_eta};
      //std::cout << gen_match_undecayed_1_pt << "    " << gen_match_undecayed_1_eta << "    " << gen_match_undecayed_2_pt << "    " << gen_match_undecayed_2_eta << std::endl;
      //std::cout << fns_["m_sel_idEmb_ratio"]->eval(args_1.data()) << "    " << fns_["m_sel_idEmb_ratio"]->eval(args_2.data()) << "    " << fns_["m_sel_trg_ratio"]->eval(args_4.data()) << std::endl;
      double wt_embedding_yield = fns_["m_sel_idEmb_ratio"]->eval(args_1.data())*fns_["m_sel_idEmb_ratio"]->eval(args_2.data())*fns_["m_sel_trg_ratio"]->eval(args_4.data());
      // global scale factors not used anymore
      //if(channel_==channel::mt)      wt_embedding_yield = 1.192;
      //if(channel_==channel::et) wt_embedding_yield = 1.25;
      //if(channel_==channel::em) wt_embedding_yield = 1.14;
      //if(channel_==channel::tt) wt_embedding_yield = 2.1;
      //
      //if(channel_==channel::mt) wt_embedding_yield = 1.2703;
      //if(channel_==channel::et) wt_embedding_yield = 1.2244;
      //if(channel_==channel::em) wt_embedding_yield = 1.2101;
      //if(channel_==channel::tt) wt_embedding_yield = 1.2005;
      //event->Add("wt_embed_mc_yield",1.13/wt_embedding_yield); 
      if (eventInfo->weight("wt_embedding") > 1) wt_embedding_yield = 0.; // values > 1 are un-physical so set yield to 0
      eventInfo->set_weight("wt_embedding_yield", wt_embedding_yield);
    }

    if (do_topquark_weights_) {
      double top_wt = 1.0;
      double top_wt_up = 1.0;
      double top_wt_down = 1.0;
      std::vector<GenParticle *> const& parts = event->GetPtrVec<GenParticle>("genParticles");
      if(era_ != era::data_2015 && era_!=era::data_2016 && era_ != era::data_2017 && era_ != era::data_2018){
        for (unsigned i = 0; i < parts.size(); ++i) {
          if (parts[i]->status() == 3 && abs(parts[i]->pdgid()) == 6) {
            double pt = parts[i]->pt();
            pt = std::min(pt, 400.);
            if (mc_ == mc::summer12_53X) top_wt *= std::exp(0.156-0.00137*pt);
            if (mc_ == mc::fall11_42X)   top_wt *= std::exp(0.199-0.00166*pt);
          }
        }
      } else {
        for (unsigned i = 0; i < parts.size(); ++i){
          std::vector<bool> status_flags = parts[i]->statusFlags();
          unsigned id = abs(parts[i]->pdgid());
          if(id == 6 && status_flags[FromHardProcess] && status_flags[IsLastCopy]){
            double pt = parts[i]->pt();
            pt = std::min(pt, 400.);
            if (mc_ == mc::fall15_76X || mc_ == mc::spring16_80X || channel_==channel::em) top_wt *= std::exp(0.156-0.00137*pt);
            //if (mc_ == mc::summer16_80X && channel_!=channel::em){
            //  //top_wt *= std::exp(0.0615-0.0005*pt); //13TeV
            //  top_wt *= std::exp(0.156-0.00137 *pt); //8TeV
            //}
            if (mc_ == mc::summer16_80X && !(mc_ == mc::mc2017 || mc_ == mc::mc2018 || mc_ == mc::mcleg2016)){
              top_wt *= std::exp(0.0615-0.0005*pt); //13TeV
            }
            if(mc_ == mc::mc2017 || mc_ == mc::mc2018 || mc_ == mc::mcleg2016) {
              pt = std::min(pt, 472.);
              double a = 0.088, b = -0.00087, c = 9.2e-07;
              top_wt *= std::exp(a + b * pt + c * pt*pt); 
            }
          }
        }
      }
          
      top_wt = std::sqrt(top_wt);
      top_wt_up = top_wt * top_wt;
      top_wt_down = 1.0;
      event->Add("wt_tquark_up", top_wt_up / top_wt);
      event->Add("wt_tquark_down", top_wt_down / top_wt);
      eventInfo->set_weight("topquark_weight", top_wt);
    }
    
    if (do_tau_fake_weights_){
      if(era_ != era::data_2016 && era_ != era::data_2017 && era_ != era::data_2018){
        Tau const* tau = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton2"));
        double fake_pt = tau->pt() < 200. ? tau->pt() : 200.;
        double fake_weight = tau_fake_weights_->Eval(fake_pt);
        eventInfo->set_weight("tau_fake_weight",fake_weight);
        double weight_up   = (fake_weight + 0.5*(1.0-fake_weight)) / fake_weight;
        double weight_down = (fake_weight - 0.5*(1.0-fake_weight)) / fake_weight;
        event->Add("wt_tau_fake_up", weight_up);
        event->Add("wt_tau_fake_down", weight_down);
      } else {
        //For 2016, use UP weight = down 20% per 100 GeV and vice-versa
        if(channel_ != channel::em){
          unsigned gen_match_2 = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_2"));
          double weight_up = 1.0;
          double weight_down = 1.0;
          double tau_1_wt=0;
          double tau_2_wt=0;
          if(gen_match_2 == 6){
            Candidate const* tau2 = dilepton[0]->GetCandidate("lepton2");
            double pt_2 = tau2->pt();
            tau_2_wt = (0.20*pt_2)/100.;
          } 
          weight_down = 1.0 + tau_2_wt;
          weight_up = std::max(0.0, 1.0 - tau_2_wt);
          if (channel_ == channel::tt){
           unsigned gen_match_1 = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_1"));
           if(gen_match_1 == 6){
             Candidate const* tau1 = dilepton[0]->GetCandidate("lepton1");
             double pt_1 = tau1->pt();
             tau_1_wt = (0.20*pt_1/100.);
           }
           weight_down = (1.0+tau_2_wt)*(1.0+tau_1_wt);
           weight_up = std::max(0.0, (1.0-tau_2_wt)*(1.0-tau_1_wt));
          }
         event->Add("wt_tau_fake_up", weight_up);
         event->Add("wt_tau_fake_down", weight_down);
        }
      }
    }

    if (do_tau_id_weights_) {
      if(era_ != era::data_2015 && era_!=era::data_2016 && era_ != era::data_2017 && era_ !=era::data_2018){
        std::vector<Candidate *> tau = { (dilepton[0]->GetCandidate("lepton2")) };
        std::vector<GenParticle *> const& particles = event->GetPtrVec<GenParticle>(gen_tau_collection_);
        std::vector<GenJet> gen_taus = BuildTauJets(particles, false,false);
        std::vector<GenJet *> gen_taus_ptr;
        for (auto & x : gen_taus) gen_taus_ptr.push_back(&x);
        std::vector<std::pair<Candidate*, GenJet*> > matches = MatchByDR(tau, gen_taus_ptr, 0.5, true, true);
        double weight_up    = 1.0;
        double weight_down  = 1.0;
        if (matches.size() == 1) {
          double weight = (0.20*matches[0].second->pt())/1000.;
          weight_up   = 1.0 +weight;
          weight_down = std::max(0.0, 1.0 - weight);
          event->Add("wt_tau_id_up", weight_up);
          event->Add("wt_tau_id_down", weight_down);
        }
      } else {
       if(channel_ != channel::em){
          if(mc_ != mc::summer16_80X && mc_ != mc::mc2017 && mc_ != mc::mc2018 && mc_ != mc::mcleg2016){
            double gen_match_2_pt = event->Get<double>("gen_match_2_pt");
            unsigned gen_match_2 = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_2"));
            double weight_up    = 1.0;
            double weight_down  = 1.0;
            double tau2_wt = 0;
            if(gen_match_2 == 5){
              tau2_wt = (0.20*gen_match_2_pt)/1000.;
            }
            weight_up   = 1.0 +tau2_wt;
            weight_down = std::max(0.0, 1.0 - tau2_wt);
            if(channel_ == channel::tt){
             double tau1_wt = 0;
             double gen_match_1_pt = event->Get<double>("gen_match_1_pt");
             unsigned gen_match_1 = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_1"));
             if(gen_match_1 == 5){
               tau1_wt = (0.20*gen_match_1_pt)/1000.;
             }
             weight_up = (1.0+tau2_wt)*(1.0+tau1_wt);
             weight_down = std::max(0.0,(1.0-tau2_wt)*(1.0-tau1_wt));
            }
            event->Add("wt_tau_id_up", weight_up);
            event->Add("wt_tau_id_down", weight_down);
          } else if(mc_ == mc::summer16_80X || mc_ == mc::mc2017 || mc_ == mc::mc2018 || mc_ == mc::mcleg2016){
            double gen_match_2_pt = event->Get<double>("gen_match_2_pt");
            unsigned gen_match_2 = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_2"));
            double weight_up    = 1.0;
            double weight_down  = 1.0;
            double tau2_wt_up = 0;
            double tau2_wt_down = 0;
            if(gen_match_2 == 5){
              tau2_wt_up = (0.05*gen_match_2_pt)/1000.;
              tau2_wt_down = (0.35*gen_match_2_pt)/1000.;
            }
            weight_up   = 1.0 +tau2_wt_up;
            weight_down = std::max(0.0, 1.0 - tau2_wt_down);
            if(channel_ == channel::tt){
             double tau1_wt_up = 0;
             double tau1_wt_down = 0;
             double gen_match_1_pt = event->Get<double>("gen_match_1_pt");
             unsigned gen_match_1 = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_1"));
             if(gen_match_1 == 5){
               tau1_wt_up = (0.05*gen_match_1_pt)/1000.;
               tau1_wt_down = (0.35*gen_match_1_pt)/1000.;
             }
             weight_up = (1.0+tau2_wt_up)*(1.0+tau1_wt_up);
             weight_down = std::max(0.0,(1.0-tau2_wt_down)*(1.0-tau1_wt_down));
            }
            event->Add("wt_tau_id_up", weight_up);
            event->Add("wt_tau_id_down", weight_down);
          }
        }   
      }
    }
    if (do_tau_id_sf_ && channel_!= channel::em  && channel_!= channel::tpzee  && channel_!= channel::tpzmm && channel_!= channel::tpmt && channel_!= channel::tpem) {
      double tau_sf_1 = 1.0;
      double tau_sf_2 = 1.0;
      if (channel_ != channel::tt){
        unsigned gen_match_2 = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_2"));
        Tau const* tau2 = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton2"));
        double decay_mode_2 = tau2->decay_mode();
        double pt_2 = tau2->pt();
        double eta_2 = tau2->eta();
        auto args_2 = std::vector<double>{pt_2,eta_2,decay_mode_2};

        double mvadm_2 = tau2->GetTauID("MVADM2017v1");
        auto args_mvadm = std::vector<double>{pt_2,mvadm_2};

        if(mc_ == mc::summer16_80X){
            tau_sf_2  = (gen_match_2 == 5 && !is_embedded_) ? 0.95 : 1.0;
            double loose_tau_sf_2 = (gen_match_2 == 5 && !is_embedded_) ? 0.99 : 1.0;
            double medium_tau_sf_2 = (gen_match_2 == 5 && !is_embedded_) ? 0.97 : 1.0;
            event->Add("wt_tau_id_loose",loose_tau_sf_2/(tau_sf_2));
            event->Add("wt_tau_id_medium",medium_tau_sf_2/(tau_sf_2));
        } else if ((mc_==mc::mc2017 || mc_==mc::mc2018 || mc_ == mc::mcleg2016) && !is_embedded_){
          auto args_pt = std::vector<double>{pt_2};
          tau_sf_2 = (gen_match_2==5) ? fns_["t_deeptauid_pt_medium"]->eval(args_pt.data()) : 1.0;


          double tau_sf_2_bin1_up = (gen_match_2==5) ? fns_["t_deeptauid_pt_medium_bin1_up"]->eval(args_pt.data())/tau_sf_2 : 1.0;
          double tau_sf_2_bin2_up = (gen_match_2==5) ? fns_["t_deeptauid_pt_medium_bin2_up"]->eval(args_pt.data())/tau_sf_2 : 1.0;
          double tau_sf_2_bin3_up = (gen_match_2==5) ? fns_["t_deeptauid_pt_medium_bin3_up"]->eval(args_pt.data())/tau_sf_2 : 1.0;
          double tau_sf_2_bin4_up = (gen_match_2==5) ? fns_["t_deeptauid_pt_medium_bin4_up"]->eval(args_pt.data())/tau_sf_2 : 1.0;
          double tau_sf_2_bin5_up = (gen_match_2==5) ? fns_["t_deeptauid_pt_medium_bin5_up"]->eval(args_pt.data())/tau_sf_2 : 1.0;
          double tau_sf_2_bin1_down = (gen_match_2==5) ? fns_["t_deeptauid_pt_medium_bin1_down"]->eval(args_pt.data())/tau_sf_2 : 1.0;
          double tau_sf_2_bin2_down = (gen_match_2==5) ? fns_["t_deeptauid_pt_medium_bin2_down"]->eval(args_pt.data())/tau_sf_2 : 1.0;
          double tau_sf_2_bin3_down = (gen_match_2==5) ? fns_["t_deeptauid_pt_medium_bin3_down"]->eval(args_pt.data())/tau_sf_2 : 1.0;
          double tau_sf_2_bin4_down = (gen_match_2==5) ? fns_["t_deeptauid_pt_medium_bin4_down"]->eval(args_pt.data())/tau_sf_2 : 1.0;
          double tau_sf_2_bin5_down = (gen_match_2==5) ? fns_["t_deeptauid_pt_medium_bin5_down"]->eval(args_pt.data())/tau_sf_2 : 1.0;

          event->Add("wt_tau_id_pt_bin1_up",tau_sf_2_bin1_up);
          event->Add("wt_tau_id_pt_bin2_up",tau_sf_2_bin2_up);
          event->Add("wt_tau_id_pt_bin3_up",tau_sf_2_bin3_up);
          event->Add("wt_tau_id_pt_bin4_up",tau_sf_2_bin4_up);
          event->Add("wt_tau_id_pt_bin5_up",tau_sf_2_bin5_up);
          event->Add("wt_tau_id_pt_bin1_down",tau_sf_2_bin1_down);
          event->Add("wt_tau_id_pt_bin2_down",tau_sf_2_bin2_down);
          event->Add("wt_tau_id_pt_bin3_down",tau_sf_2_bin3_down);
          event->Add("wt_tau_id_pt_bin4_down",tau_sf_2_bin4_down);
          event->Add("wt_tau_id_pt_bin5_down",tau_sf_2_bin5_down);

          double tau_sf_mvadm_2 = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium"]->eval(args_mvadm.data()) : 1.0;

          double tau_sf_mvadm_2_lowpt_mvadm0_up = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_lowpt_mvadm0_up"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
          double tau_sf_mvadm_2_lowpt_mvadm1_up = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_lowpt_mvadm1_up"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
          double tau_sf_mvadm_2_lowpt_mvadm2_up = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_lowpt_mvadm2_up"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
          double tau_sf_mvadm_2_lowpt_mvadm10_up = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_lowpt_mvadm10_up"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
          double tau_sf_mvadm_2_lowpt_mvadm11_up = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_lowpt_mvadm11_up"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
          double tau_sf_mvadm_2_lowpt_mvadm0_down = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_lowpt_mvadm0_down"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
          double tau_sf_mvadm_2_lowpt_mvadm1_down = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_lowpt_mvadm1_down"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
          double tau_sf_mvadm_2_lowpt_mvadm2_down = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_lowpt_mvadm2_down"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
          double tau_sf_mvadm_2_lowpt_mvadm10_down = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_lowpt_mvadm10_down"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
          double tau_sf_mvadm_2_lowpt_mvadm11_down = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_lowpt_mvadm11_down"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
          double tau_sf_mvadm_2_highpt_mvadm0_up = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_highpt_mvadm0_up"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
          double tau_sf_mvadm_2_highpt_mvadm1_up = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_highpt_mvadm1_up"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
          double tau_sf_mvadm_2_highpt_mvadm2_up = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_highpt_mvadm2_up"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
          double tau_sf_mvadm_2_highpt_mvadm10_up = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_highpt_mvadm10_up"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
          double tau_sf_mvadm_2_highpt_mvadm11_up = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_highpt_mvadm11_up"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
          double tau_sf_mvadm_2_highpt_mvadm0_down = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_highpt_mvadm0_down"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
          double tau_sf_mvadm_2_highpt_mvadm1_down = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_highpt_mvadm1_down"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
          double tau_sf_mvadm_2_highpt_mvadm2_down = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_highpt_mvadm2_down"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
          double tau_sf_mvadm_2_highpt_mvadm10_down = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_highpt_mvadm10_down"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
          double tau_sf_mvadm_2_highpt_mvadm11_down = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_highpt_mvadm11_down"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;

          event->Add("wt_tau_id_mvadm",tau_sf_mvadm_2/tau_sf_2);

          event->Add("wt_tau_id_lowpt_mvadm0_up",tau_sf_mvadm_2_lowpt_mvadm0_up); 
          event->Add("wt_tau_id_lowpt_mvadm1_up",tau_sf_mvadm_2_lowpt_mvadm1_up); 
          event->Add("wt_tau_id_lowpt_mvadm2_up",tau_sf_mvadm_2_lowpt_mvadm2_up); 
          event->Add("wt_tau_id_lowpt_mvadm10_up",tau_sf_mvadm_2_lowpt_mvadm10_up); 
          event->Add("wt_tau_id_lowpt_mvadm11_up",tau_sf_mvadm_2_lowpt_mvadm11_up); 
          event->Add("wt_tau_id_highpt_mvadm0_up",tau_sf_mvadm_2_highpt_mvadm0_up);
          event->Add("wt_tau_id_highpt_mvadm1_up",tau_sf_mvadm_2_highpt_mvadm1_up);
          event->Add("wt_tau_id_highpt_mvadm2_up",tau_sf_mvadm_2_highpt_mvadm2_up);
          event->Add("wt_tau_id_highpt_mvadm10_up",tau_sf_mvadm_2_highpt_mvadm10_up);
          event->Add("wt_tau_id_highpt_mvadm11_up",tau_sf_mvadm_2_highpt_mvadm11_up);
          event->Add("wt_tau_id_lowpt_mvadm0_down",tau_sf_mvadm_2_lowpt_mvadm0_down);
          event->Add("wt_tau_id_lowpt_mvadm1_down",tau_sf_mvadm_2_lowpt_mvadm1_down);
          event->Add("wt_tau_id_lowpt_mvadm2_down",tau_sf_mvadm_2_lowpt_mvadm2_down);
          event->Add("wt_tau_id_lowpt_mvadm10_down",tau_sf_mvadm_2_lowpt_mvadm10_down);
          event->Add("wt_tau_id_lowpt_mvadm11_down",tau_sf_mvadm_2_lowpt_mvadm11_down);
          event->Add("wt_tau_id_highpt_mvadm0_down",tau_sf_mvadm_2_highpt_mvadm0_down);
          event->Add("wt_tau_id_highpt_mvadm1_down",tau_sf_mvadm_2_highpt_mvadm1_down);
          event->Add("wt_tau_id_highpt_mvadm2_down",tau_sf_mvadm_2_highpt_mvadm2_down);
          event->Add("wt_tau_id_highpt_mvadm10_down",tau_sf_mvadm_2_highpt_mvadm10_down);
          event->Add("wt_tau_id_highpt_mvadm11_down",tau_sf_mvadm_2_highpt_mvadm11_down);
        }
        else if ((mc_==mc::mc2017 || mc_==mc::mc2018 || mc_ == mc::mcleg2016) && is_embedded_) {

          auto args_pt = std::vector<double>{pt_2};
          tau_sf_2 = (gen_match_2==5) ? fns_["t_deeptauid_pt_embed_medium"]->eval(args_pt.data()) : 1.0;

          if(channel_== channel::et) {
            double et_tau_extra = (gen_match_2==5) ? fns_["t_deeptauid_pt_tightvse_embed_medium"]->eval(args_pt.data()) : 1.0;
            et_tau_extra/=tau_sf_2;
            event->Add("wt_tau_id_extra",et_tau_extra);
          }

          double tau_sf_2_bin1_up = (gen_match_2==5) ? fns_["t_deeptauid_pt_embed_medium_bin1_up"]->eval(args_pt.data())/tau_sf_2 : 1.0;
          double tau_sf_2_bin2_up = (gen_match_2==5) ? fns_["t_deeptauid_pt_embed_medium_bin2_up"]->eval(args_pt.data())/tau_sf_2 : 1.0;
          double tau_sf_2_bin3_up = (gen_match_2==5) ? fns_["t_deeptauid_pt_embed_medium_bin3_up"]->eval(args_pt.data())/tau_sf_2 : 1.0;
          double tau_sf_2_bin4_up = (gen_match_2==5) ? fns_["t_deeptauid_pt_embed_medium_bin4_up"]->eval(args_pt.data())/tau_sf_2 : 1.0;
          double tau_sf_2_bin5_up = (gen_match_2==5) ? fns_["t_deeptauid_pt_embed_medium_bin5_up"]->eval(args_pt.data())/tau_sf_2 : 1.0;
          double tau_sf_2_bin1_down = (gen_match_2==5) ? fns_["t_deeptauid_pt_embed_medium_bin1_down"]->eval(args_pt.data())/tau_sf_2 : 1.0;
          double tau_sf_2_bin2_down = (gen_match_2==5) ? fns_["t_deeptauid_pt_embed_medium_bin2_down"]->eval(args_pt.data())/tau_sf_2 : 1.0;
          double tau_sf_2_bin3_down = (gen_match_2==5) ? fns_["t_deeptauid_pt_embed_medium_bin3_down"]->eval(args_pt.data())/tau_sf_2 : 1.0;
          double tau_sf_2_bin4_down = (gen_match_2==5) ? fns_["t_deeptauid_pt_embed_medium_bin4_down"]->eval(args_pt.data())/tau_sf_2 : 1.0;
          double tau_sf_2_bin5_down = (gen_match_2==5) ? fns_["t_deeptauid_pt_embed_medium_bin5_down"]->eval(args_pt.data())/tau_sf_2 : 1.0;

          if(gen_match_2==5) {
            if (decay_mode_2==0) tau_sf_2*=0.975;
            else if (decay_mode_2==1) tau_sf_2*=0.975*1.051;
            else if (decay_mode_2==5||decay_mode_2==6) tau_sf_2*=pow(0.975,2);
            else if (decay_mode_2==10) tau_sf_2*=pow(0.975,3);
            else if (decay_mode_2==11) tau_sf_2*=pow(0.975,3)*1.051;
          }

          event->Add("wt_tau_id_pt_bin1_up",tau_sf_2_bin1_up);
          event->Add("wt_tau_id_pt_bin2_up",tau_sf_2_bin2_up);
          event->Add("wt_tau_id_pt_bin3_up",tau_sf_2_bin3_up);
          event->Add("wt_tau_id_pt_bin4_up",tau_sf_2_bin4_up);
          event->Add("wt_tau_id_pt_bin5_up",tau_sf_2_bin5_up);
          event->Add("wt_tau_id_pt_bin1_down",tau_sf_2_bin1_down);
          event->Add("wt_tau_id_pt_bin2_down",tau_sf_2_bin2_down);
          event->Add("wt_tau_id_pt_bin3_down",tau_sf_2_bin3_down);
          event->Add("wt_tau_id_pt_bin4_down",tau_sf_2_bin4_down);
          event->Add("wt_tau_id_pt_bin5_down",tau_sf_2_bin5_down);

          double tau_sf_mvadm_2 = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium"]->eval(args_mvadm.data()) : 1.0;

          double tau_sf_mvadm_2_lowpt_mvadm0_up = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm0_up"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
          double tau_sf_mvadm_2_lowpt_mvadm1_up = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm1_up"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
          double tau_sf_mvadm_2_lowpt_mvadm2_up = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm2_up"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
          double tau_sf_mvadm_2_lowpt_mvadm10_up = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm10_up"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
          double tau_sf_mvadm_2_lowpt_mvadm11_up = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm11_up"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
          double tau_sf_mvadm_2_lowpt_mvadm0_down = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm0_down"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
          double tau_sf_mvadm_2_lowpt_mvadm1_down = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm1_down"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
          double tau_sf_mvadm_2_lowpt_mvadm2_down = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm2_down"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
          double tau_sf_mvadm_2_lowpt_mvadm10_down = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm10_down"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
          double tau_sf_mvadm_2_lowpt_mvadm11_down = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm11_down"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
          double tau_sf_mvadm_2_highpt_mvadm0_up = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm0_up"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
          double tau_sf_mvadm_2_highpt_mvadm1_up = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm1_up"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
          double tau_sf_mvadm_2_highpt_mvadm2_up = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm2_up"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
          double tau_sf_mvadm_2_highpt_mvadm10_up = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm10_up"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
          double tau_sf_mvadm_2_highpt_mvadm11_up = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm11_up"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
          double tau_sf_mvadm_2_highpt_mvadm0_down = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm0_down"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
          double tau_sf_mvadm_2_highpt_mvadm1_down = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm1_down"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
          double tau_sf_mvadm_2_highpt_mvadm2_down = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm2_down"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
          double tau_sf_mvadm_2_highpt_mvadm10_down = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm10_down"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
          double tau_sf_mvadm_2_highpt_mvadm11_down = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm11_down"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;

          event->Add("wt_tau_id_mvadm",tau_sf_mvadm_2/tau_sf_2);

          event->Add("wt_tau_id_lowpt_mvadm0_up",tau_sf_mvadm_2_lowpt_mvadm0_up);
          event->Add("wt_tau_id_lowpt_mvadm1_up",tau_sf_mvadm_2_lowpt_mvadm1_up);
          event->Add("wt_tau_id_lowpt_mvadm2_up",tau_sf_mvadm_2_lowpt_mvadm2_up);
          event->Add("wt_tau_id_lowpt_mvadm10_up",tau_sf_mvadm_2_lowpt_mvadm10_up);
          event->Add("wt_tau_id_lowpt_mvadm11_up",tau_sf_mvadm_2_lowpt_mvadm11_up);
          event->Add("wt_tau_id_highpt_mvadm0_up",tau_sf_mvadm_2_highpt_mvadm0_up);
          event->Add("wt_tau_id_highpt_mvadm1_up",tau_sf_mvadm_2_highpt_mvadm1_up);
          event->Add("wt_tau_id_highpt_mvadm2_up",tau_sf_mvadm_2_highpt_mvadm2_up);
          event->Add("wt_tau_id_highpt_mvadm10_up",tau_sf_mvadm_2_highpt_mvadm10_up);
          event->Add("wt_tau_id_highpt_mvadm11_up",tau_sf_mvadm_2_highpt_mvadm11_up);
          event->Add("wt_tau_id_lowpt_mvadm0_down",tau_sf_mvadm_2_lowpt_mvadm0_down);
          event->Add("wt_tau_id_lowpt_mvadm1_down",tau_sf_mvadm_2_lowpt_mvadm1_down);
          event->Add("wt_tau_id_lowpt_mvadm2_down",tau_sf_mvadm_2_lowpt_mvadm2_down);
          event->Add("wt_tau_id_lowpt_mvadm10_down",tau_sf_mvadm_2_lowpt_mvadm10_down);
          event->Add("wt_tau_id_lowpt_mvadm11_down",tau_sf_mvadm_2_lowpt_mvadm11_down);
          event->Add("wt_tau_id_highpt_mvadm0_down",tau_sf_mvadm_2_highpt_mvadm0_down);
          event->Add("wt_tau_id_highpt_mvadm1_down",tau_sf_mvadm_2_highpt_mvadm1_down);
          event->Add("wt_tau_id_highpt_mvadm2_down",tau_sf_mvadm_2_highpt_mvadm2_down);
          event->Add("wt_tau_id_highpt_mvadm10_down",tau_sf_mvadm_2_highpt_mvadm10_down);
          event->Add("wt_tau_id_highpt_mvadm11_down",tau_sf_mvadm_2_highpt_mvadm11_down);

        }
        else tau_sf_2 =  (gen_match_2 == 5) ? fns_["t_iso_mva_m_pt30_sf"]->eval(args_2.data()) : 1.0;
      } else {
        unsigned gen_match_2 = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_2"));
        unsigned gen_match_1 = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_1"));
        Tau const* tau1 = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton1"));
        double decay_mode_1 = tau1->decay_mode();
        double pt_1 = tau1->pt();
        double eta_1 = tau1->eta();
        Tau const* tau2 = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton2"));
        double decay_mode_2 = tau2->decay_mode();
        double pt_2 = tau2->pt();
        double eta_2 = tau2->eta();
        auto args_1 = std::vector<double>{pt_1,eta_1,decay_mode_1};
        auto args_2 = std::vector<double>{pt_2,eta_2,decay_mode_2};

        double mvadm_1 = tau1->GetTauID("MVADM2017v1");
        double mvadm_2 = tau2->GetTauID("MVADM2017v1");

        auto args_mvadm_1 = std::vector<double>{pt_1,mvadm_1};
        auto args_mvadm_2 = std::vector<double>{pt_2,mvadm_2};

        double tau_sf_dm0_up=1.;
        double tau_sf_dm1_up=1.;
        double tau_sf_dm10_up=1.;
        double tau_sf_dm11_up=1.;
        double tau_sf_dm0_down=1.;
        double tau_sf_dm1_down=1.;
        double tau_sf_dm10_down=1.;
        double tau_sf_dm11_down=1.;

        double tau_sf_mvadm_1=1.;
        double tau_sf_mvadm_2=1.;

        double tau_sf_mvadm_lowpt_mvadm0_up=1.;
        double tau_sf_mvadm_lowpt_mvadm1_up=1.;
        double tau_sf_mvadm_lowpt_mvadm2_up=1.;
        double tau_sf_mvadm_lowpt_mvadm10_up=1.;
        double tau_sf_mvadm_lowpt_mvadm11_up=1.;
        double tau_sf_mvadm_lowpt_mvadm0_down=1.;
        double tau_sf_mvadm_lowpt_mvadm1_down=1.;
        double tau_sf_mvadm_lowpt_mvadm2_down=1.;
        double tau_sf_mvadm_lowpt_mvadm10_down=1.;
        double tau_sf_mvadm_lowpt_mvadm11_down=1.;
        double tau_sf_mvadm_highpt_mvadm0_up=1.;
        double tau_sf_mvadm_highpt_mvadm1_up=1.;
        double tau_sf_mvadm_highpt_mvadm2_up=1.;
        double tau_sf_mvadm_highpt_mvadm10_up=1.;
        double tau_sf_mvadm_highpt_mvadm11_up=1.;
        double tau_sf_mvadm_highpt_mvadm0_down=1.;
        double tau_sf_mvadm_highpt_mvadm1_down=1.;
        double tau_sf_mvadm_highpt_mvadm2_down=1.;
        double tau_sf_mvadm_highpt_mvadm10_down=1.;
        double tau_sf_mvadm_highpt_mvadm11_down=1.;

        if(mc_ == mc::summer16_80X){
          if (strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16 || strategy_ == strategy::legacy16 || strategy_ == strategy::cpdecays16) tau_sf_1  = (gen_match_1 == 5 && !is_embedded_) ? 0.95 : 1.0;  
          else tau_sf_1  = (gen_match_1 == 5 && !is_embedded_) ? 0.97 : 1.0;
          if(is_embedded_ && gen_match_1 == 5){
            tau_sf_1=1.02;
            if (decay_mode_1==0)       tau_sf_1*=0.975;
            else if (decay_mode_1==1)  tau_sf_1*=0.975*1.051;
            else if (decay_mode_1>3) tau_sf_1*=pow(0.975,3);
          } else tau_sf_1 = 1.0;
        } else if ((mc_==mc::mc2017 || mc_==mc::mc2018 || mc_ == mc::mcleg2016) && !is_embedded_){
          auto args_dm_1 = std::vector<double>{decay_mode_1};
          tau_sf_1 = (gen_match_1==5) ? fns_["t_deeptauid_dm_medium"]->eval(args_dm_1.data()) : 1.0;
          tau_sf_dm0_up*=((gen_match_1==5) ? fns_["t_deeptauid_dm_medium_dm0_up"]->eval(args_dm_1.data()) : 1.0)/tau_sf_1;
          tau_sf_dm1_up*=((gen_match_1==5) ? fns_["t_deeptauid_dm_medium_dm1_up"]->eval(args_dm_1.data()) : 1.0)/tau_sf_1;
          tau_sf_dm10_up*=((gen_match_1==5) ? fns_["t_deeptauid_dm_medium_dm10_up"]->eval(args_dm_1.data()) : 1.0)/tau_sf_1;
          tau_sf_dm11_up*=((gen_match_1==5) ? fns_["t_deeptauid_dm_medium_dm11_up"]->eval(args_dm_1.data()) : 1.0)/tau_sf_1;
          tau_sf_dm0_down*=((gen_match_1==5) ? fns_["t_deeptauid_dm_medium_dm0_down"]->eval(args_dm_1.data()) : 1.0)/tau_sf_1;
          tau_sf_dm1_down*=((gen_match_1==5) ? fns_["t_deeptauid_dm_medium_dm1_down"]->eval(args_dm_1.data()) : 1.0)/tau_sf_1;
          tau_sf_dm10_down*=((gen_match_1==5) ? fns_["t_deeptauid_dm_medium_dm10_down"]->eval(args_dm_1.data()) : 1.0)/tau_sf_1;
          tau_sf_dm11_down*=((gen_match_1==5) ? fns_["t_deeptauid_dm_medium_dm11_down"]->eval(args_dm_1.data()) : 1.0)/tau_sf_1;

          tau_sf_mvadm_1 = (gen_match_1==5) ? fns_["t_deeptauid_mvadm_medium"]->eval(args_mvadm_1.data()) : 1.0;

          tau_sf_mvadm_lowpt_mvadm0_up *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_medium_lowpt_mvadm0_up"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
          tau_sf_mvadm_lowpt_mvadm1_up *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_medium_lowpt_mvadm1_up"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
          tau_sf_mvadm_lowpt_mvadm2_up *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_medium_lowpt_mvadm2_up"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
          tau_sf_mvadm_lowpt_mvadm10_up *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_medium_lowpt_mvadm10_up"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
          tau_sf_mvadm_lowpt_mvadm11_up *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_medium_lowpt_mvadm11_up"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
          tau_sf_mvadm_lowpt_mvadm0_down *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_medium_lowpt_mvadm0_down"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
          tau_sf_mvadm_lowpt_mvadm1_down *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_medium_lowpt_mvadm1_down"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
          tau_sf_mvadm_lowpt_mvadm2_down *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_medium_lowpt_mvadm2_down"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
          tau_sf_mvadm_lowpt_mvadm10_down *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_medium_lowpt_mvadm10_down"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
          tau_sf_mvadm_lowpt_mvadm11_down *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_medium_lowpt_mvadm11_down"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
          tau_sf_mvadm_highpt_mvadm0_up *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_medium_highpt_mvadm0_up"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
          tau_sf_mvadm_highpt_mvadm1_up *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_medium_highpt_mvadm1_up"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
          tau_sf_mvadm_highpt_mvadm2_up *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_medium_highpt_mvadm2_up"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
          tau_sf_mvadm_highpt_mvadm10_up *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_medium_highpt_mvadm10_up"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
          tau_sf_mvadm_highpt_mvadm11_up *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_medium_highpt_mvadm11_up"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
          tau_sf_mvadm_highpt_mvadm0_down *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_medium_highpt_mvadm0_down"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
          tau_sf_mvadm_highpt_mvadm1_down *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_medium_highpt_mvadm1_down"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
          tau_sf_mvadm_highpt_mvadm2_down *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_medium_highpt_mvadm2_down"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
          tau_sf_mvadm_highpt_mvadm10_down *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_medium_highpt_mvadm10_down"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
          tau_sf_mvadm_highpt_mvadm11_down *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_medium_highpt_mvadm11_down"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;

        }
        else if ((mc_==mc::mc2017 || mc_==mc::mc2018 || mc_ == mc::mcleg2016) && is_embedded_) {
          auto args_dm_1 = std::vector<double>{decay_mode_1};
          tau_sf_1 = (gen_match_1==5) ? fns_["t_deeptauid_dm_embed_medium"]->eval(args_dm_1.data()) : 1.0;
          tau_sf_dm0_up*=((gen_match_1==5) ? fns_["t_deeptauid_dm_embed_medium_dm0_up"]->eval(args_dm_1.data()) : 1.0)/tau_sf_1;
          tau_sf_dm1_up*=((gen_match_1==5) ? fns_["t_deeptauid_dm_embed_medium_dm1_up"]->eval(args_dm_1.data()) : 1.0)/tau_sf_1;
          tau_sf_dm10_up*=((gen_match_1==5) ? fns_["t_deeptauid_dm_embed_medium_dm10_up"]->eval(args_dm_1.data()) : 1.0)/tau_sf_1;
          tau_sf_dm11_up*=((gen_match_1==5) ? fns_["t_deeptauid_dm_embed_medium_dm11_up"]->eval(args_dm_1.data()) : 1.0)/tau_sf_1;
          tau_sf_dm0_down*=((gen_match_1==5) ? fns_["t_deeptauid_dm_embed_medium_dm0_down"]->eval(args_dm_1.data()) : 1.0)/tau_sf_1;
          tau_sf_dm1_down*=((gen_match_1==5) ? fns_["t_deeptauid_dm_embed_medium_dm1_down"]->eval(args_dm_1.data()) : 1.0)/tau_sf_1;
          tau_sf_dm10_down*=((gen_match_1==5) ? fns_["t_deeptauid_dm_embed_medium_dm10_down"]->eval(args_dm_1.data()) : 1.0)/tau_sf_1;
          tau_sf_dm11_down*=((gen_match_1==5) ? fns_["t_deeptauid_dm_embed_medium_dm11_down"]->eval(args_dm_1.data()) : 1.0)/tau_sf_1;

          if(gen_match_1==5) {
            if (decay_mode_1==0)       tau_sf_1*=0.975;
            else if (decay_mode_1==1)  tau_sf_1*=0.975*1.051;
            else if (decay_mode_1==5||decay_mode_1==6) tau_sf_1*=pow(0.975,2);
            else if (decay_mode_1==10) tau_sf_1*=pow(0.975,3);
            else if (decay_mode_1==11) tau_sf_1*=pow(0.975,3)*1.051;
          }

          tau_sf_mvadm_1 = (gen_match_1==5) ? fns_["t_deeptauid_mvadm_embed_medium"]->eval(args_mvadm_1.data()) : 1.0;

          tau_sf_mvadm_lowpt_mvadm0_up *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm0_up"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
          tau_sf_mvadm_lowpt_mvadm1_up *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm1_up"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
          tau_sf_mvadm_lowpt_mvadm2_up *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm2_up"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
          tau_sf_mvadm_lowpt_mvadm10_up *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm10_up"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
          tau_sf_mvadm_lowpt_mvadm11_up *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm11_up"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
          tau_sf_mvadm_lowpt_mvadm0_down *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm0_down"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
          tau_sf_mvadm_lowpt_mvadm1_down *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm1_down"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
          tau_sf_mvadm_lowpt_mvadm2_down *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm2_down"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
          tau_sf_mvadm_lowpt_mvadm10_down *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm10_down"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
          tau_sf_mvadm_lowpt_mvadm11_down *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm11_down"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
          tau_sf_mvadm_highpt_mvadm0_up *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm0_up"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
          tau_sf_mvadm_highpt_mvadm1_up *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm1_up"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
          tau_sf_mvadm_highpt_mvadm2_up *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm2_up"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
          tau_sf_mvadm_highpt_mvadm10_up *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm10_up"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
          tau_sf_mvadm_highpt_mvadm11_up *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm11_up"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
          tau_sf_mvadm_highpt_mvadm0_down *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm0_down"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
          tau_sf_mvadm_highpt_mvadm1_down *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm1_down"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
          tau_sf_mvadm_highpt_mvadm2_down *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm2_down"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
          tau_sf_mvadm_highpt_mvadm10_down *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm10_down"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
          tau_sf_mvadm_highpt_mvadm11_down *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm11_down"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;

        }
        else tau_sf_1 = (gen_match_1==5) ? fns_["t_iso_mva_t_pt40_eta2p1_sf"]->eval(args_1.data()) : 1.0;
        if(mc_ == mc::summer16_80X){
          if(strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16 || strategy_ == strategy::legacy16 || strategy_ == strategy::cpdecays16) tau_sf_2  = (gen_match_2 == 5 && !is_embedded_) ? 0.95 : 1.0;
          else tau_sf_2  = (gen_match_2 == 5 && !is_embedded_) ? 0.97 : 1.0;
          if(is_embedded_ && gen_match_2 == 5){
            tau_sf_2=1.02;
            if (decay_mode_2==0)       tau_sf_2*=0.975;
            else if (decay_mode_2==1)  tau_sf_2*=0.975*1.051;
            else if (decay_mode_2==5||decay_mode_2==6) tau_sf_2*=pow(0.975,2);
            else if (decay_mode_2==10) tau_sf_2*=pow(0.975,3);
            else if (decay_mode_2==11) tau_sf_2*=pow(0.975,3)*1.051;
          } else tau_sf_2 = 1.0;
        } 
        else if ((mc_==mc::mc2017 || mc_==mc::mc2018 || mc_ == mc::mcleg2016) && !is_embedded_){
          auto args_dm_2 = std::vector<double>{decay_mode_2};
          tau_sf_2 = (gen_match_2==5) ? fns_["t_deeptauid_dm_medium"]->eval(args_dm_2.data()) : 1.0;
          tau_sf_dm0_up*=((gen_match_2==5) ? fns_["t_deeptauid_dm_medium_dm0_up"]->eval(args_dm_2.data()) : 1.0)/tau_sf_2;
          tau_sf_dm1_up*=((gen_match_2==5) ? fns_["t_deeptauid_dm_medium_dm1_up"]->eval(args_dm_2.data()) : 1.0)/tau_sf_2;
          tau_sf_dm10_up*=((gen_match_2==5) ? fns_["t_deeptauid_dm_medium_dm10_up"]->eval(args_dm_2.data()) : 1.0)/tau_sf_2;
          tau_sf_dm11_up*=((gen_match_2==5) ? fns_["t_deeptauid_dm_medium_dm11_up"]->eval(args_dm_2.data()) : 1.0)/tau_sf_2;
          tau_sf_dm0_down*=((gen_match_2==5) ? fns_["t_deeptauid_dm_medium_dm0_down"]->eval(args_dm_2.data()) : 1.0)/tau_sf_2;
          tau_sf_dm1_down*=((gen_match_2==5) ? fns_["t_deeptauid_dm_medium_dm1_down"]->eval(args_dm_2.data()) : 1.0)/tau_sf_2;
          tau_sf_dm10_down*=((gen_match_2==5) ? fns_["t_deeptauid_dm_medium_dm10_down"]->eval(args_dm_2.data()) : 1.0)/tau_sf_2;
          tau_sf_dm11_down*=((gen_match_2==5) ? fns_["t_deeptauid_dm_medium_dm11_down"]->eval(args_dm_2.data()) : 1.0)/tau_sf_2;

          tau_sf_mvadm_2 = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium"]->eval(args_mvadm_2.data()) : 1.0;

          tau_sf_mvadm_lowpt_mvadm0_up *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_lowpt_mvadm0_up"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
          tau_sf_mvadm_lowpt_mvadm1_up *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_lowpt_mvadm1_up"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
          tau_sf_mvadm_lowpt_mvadm2_up *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_lowpt_mvadm2_up"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
          tau_sf_mvadm_lowpt_mvadm10_up *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_lowpt_mvadm10_up"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
          tau_sf_mvadm_lowpt_mvadm11_up *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_lowpt_mvadm11_up"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
          tau_sf_mvadm_lowpt_mvadm0_down *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_lowpt_mvadm0_down"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
          tau_sf_mvadm_lowpt_mvadm1_down *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_lowpt_mvadm1_down"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
          tau_sf_mvadm_lowpt_mvadm2_down *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_lowpt_mvadm2_down"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
          tau_sf_mvadm_lowpt_mvadm10_down *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_lowpt_mvadm10_down"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
          tau_sf_mvadm_lowpt_mvadm11_down *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_lowpt_mvadm11_down"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
          tau_sf_mvadm_highpt_mvadm0_up *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_highpt_mvadm0_up"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
          tau_sf_mvadm_highpt_mvadm1_up *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_highpt_mvadm1_up"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
          tau_sf_mvadm_highpt_mvadm2_up *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_highpt_mvadm2_up"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
          tau_sf_mvadm_highpt_mvadm10_up *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_highpt_mvadm10_up"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
          tau_sf_mvadm_highpt_mvadm11_up *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_highpt_mvadm11_up"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
          tau_sf_mvadm_highpt_mvadm0_down *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_highpt_mvadm0_down"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
          tau_sf_mvadm_highpt_mvadm1_down *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_highpt_mvadm1_down"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
          tau_sf_mvadm_highpt_mvadm2_down *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_highpt_mvadm2_down"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
          tau_sf_mvadm_highpt_mvadm10_down *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_highpt_mvadm10_down"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
          tau_sf_mvadm_highpt_mvadm11_down *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_highpt_mvadm11_down"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;

        }
	else if ((mc_==mc::mc2017 || mc_==mc::mc2018 || mc_ == mc::mcleg2016) && is_embedded_) {
          auto args_dm_2 = std::vector<double>{decay_mode_2};
          tau_sf_2 = (gen_match_2==5) ? fns_["t_deeptauid_dm_embed_medium"]->eval(args_dm_2.data()) : 1.0;
          tau_sf_dm0_up*=((gen_match_2==5) ? fns_["t_deeptauid_dm_embed_medium_dm0_up"]->eval(args_dm_2.data()) : 1.0)/tau_sf_2;
          tau_sf_dm1_up*=((gen_match_2==5) ? fns_["t_deeptauid_dm_embed_medium_dm1_up"]->eval(args_dm_2.data()) : 1.0)/tau_sf_2;
          tau_sf_dm10_up*=((gen_match_2==5) ? fns_["t_deeptauid_dm_embed_medium_dm10_up"]->eval(args_dm_2.data()) : 1.0)/tau_sf_2;
          tau_sf_dm11_up*=((gen_match_2==5) ? fns_["t_deeptauid_dm_embed_medium_dm11_up"]->eval(args_dm_2.data()) : 1.0)/tau_sf_2;
          tau_sf_dm0_down*=((gen_match_2==5) ? fns_["t_deeptauid_dm_embed_medium_dm0_down"]->eval(args_dm_2.data()) : 1.0)/tau_sf_2;
          tau_sf_dm1_down*=((gen_match_2==5) ? fns_["t_deeptauid_dm_embed_medium_dm1_down"]->eval(args_dm_2.data()) : 1.0)/tau_sf_2;
          tau_sf_dm10_down*=((gen_match_2==5) ? fns_["t_deeptauid_dm_embed_medium_dm10_down"]->eval(args_dm_2.data()) : 1.0)/tau_sf_2;
          tau_sf_dm11_down*=((gen_match_2==5) ? fns_["t_deeptauid_dm_embed_medium_dm11_down"]->eval(args_dm_2.data()) : 1.0)/tau_sf_2;

          if(gen_match_2==5) {
            if (decay_mode_2==0)       tau_sf_2*=0.975;
            else if (decay_mode_2==1)  tau_sf_2*=0.975*1.051;
            else if (decay_mode_2==5||decay_mode_2==6) tau_sf_2*=pow(0.975,2);
            else if (decay_mode_2==10) tau_sf_2*=pow(0.975,3);
            else if (decay_mode_2==11) tau_sf_2*=pow(0.975,3)*1.051;
          }

          tau_sf_mvadm_2 = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium"]->eval(args_mvadm_2.data()) : 1.0;

          tau_sf_mvadm_lowpt_mvadm0_up *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm0_up"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
          tau_sf_mvadm_lowpt_mvadm1_up *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm1_up"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
          tau_sf_mvadm_lowpt_mvadm2_up *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm2_up"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
          tau_sf_mvadm_lowpt_mvadm10_up *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm10_up"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
          tau_sf_mvadm_lowpt_mvadm11_up *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm11_up"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
          tau_sf_mvadm_lowpt_mvadm0_down *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm0_down"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
          tau_sf_mvadm_lowpt_mvadm1_down *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm1_down"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
          tau_sf_mvadm_lowpt_mvadm2_down *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm2_down"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
          tau_sf_mvadm_lowpt_mvadm10_down *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm10_down"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
          tau_sf_mvadm_lowpt_mvadm11_down *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm11_down"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
          tau_sf_mvadm_highpt_mvadm0_up *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm0_up"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
          tau_sf_mvadm_highpt_mvadm1_up *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm1_up"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
          tau_sf_mvadm_highpt_mvadm2_up *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm2_up"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
          tau_sf_mvadm_highpt_mvadm10_up *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm10_up"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
          tau_sf_mvadm_highpt_mvadm11_up *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm11_up"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
          tau_sf_mvadm_highpt_mvadm0_down *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm0_down"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
          tau_sf_mvadm_highpt_mvadm1_down *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm1_down"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
          tau_sf_mvadm_highpt_mvadm2_down *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm2_down"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
          tau_sf_mvadm_highpt_mvadm10_down *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm10_down"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
          tau_sf_mvadm_highpt_mvadm11_down *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm11_down"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;

        }
        else tau_sf_2 = (gen_match_2==5) ? fns_["t_iso_mva_t_pt40_eta2p1_sf"]->eval(args_2.data()) : 1.0;

        event->Add("wt_tau_id_dm0_up",tau_sf_dm0_up);
        event->Add("wt_tau_id_dm1_up",tau_sf_dm1_up);
        event->Add("wt_tau_id_dm10_up",tau_sf_dm10_up);
        event->Add("wt_tau_id_dm11_up",tau_sf_dm11_up);
        event->Add("wt_tau_id_dm0_down",tau_sf_dm0_down);
        event->Add("wt_tau_id_dm1_down",tau_sf_dm1_down);
        event->Add("wt_tau_id_dm10_down",tau_sf_dm10_down);
        event->Add("wt_tau_id_dm11_down",tau_sf_dm11_down);      

        event->Add("wt_tau_id_mvadm",(tau_sf_mvadm_1*tau_sf_mvadm_2)/(tau_sf_1*tau_sf_2));

        event->Add("wt_tau_id_lowpt_mvadm0_up",tau_sf_mvadm_lowpt_mvadm0_up);
        event->Add("wt_tau_id_lowpt_mvadm1_up",tau_sf_mvadm_lowpt_mvadm1_up);
        event->Add("wt_tau_id_lowpt_mvadm2_up",tau_sf_mvadm_lowpt_mvadm2_up);
        event->Add("wt_tau_id_lowpt_mvadm10_up",tau_sf_mvadm_lowpt_mvadm10_up);
        event->Add("wt_tau_id_lowpt_mvadm11_up",tau_sf_mvadm_lowpt_mvadm11_up);
        event->Add("wt_tau_id_highpt_mvadm0_up",tau_sf_mvadm_highpt_mvadm0_up);
        event->Add("wt_tau_id_highpt_mvadm1_up",tau_sf_mvadm_highpt_mvadm1_up);
        event->Add("wt_tau_id_highpt_mvadm2_up",tau_sf_mvadm_highpt_mvadm2_up);
        event->Add("wt_tau_id_highpt_mvadm10_up",tau_sf_mvadm_highpt_mvadm10_up);
        event->Add("wt_tau_id_highpt_mvadm11_up",tau_sf_mvadm_highpt_mvadm11_up);
        event->Add("wt_tau_id_lowpt_mvadm0_down",tau_sf_mvadm_lowpt_mvadm0_down);
        event->Add("wt_tau_id_lowpt_mvadm1_down",tau_sf_mvadm_lowpt_mvadm1_down);
        event->Add("wt_tau_id_lowpt_mvadm2_down",tau_sf_mvadm_lowpt_mvadm2_down);
        event->Add("wt_tau_id_lowpt_mvadm10_down",tau_sf_mvadm_lowpt_mvadm10_down);
        event->Add("wt_tau_id_lowpt_mvadm11_down",tau_sf_mvadm_lowpt_mvadm11_down);
        event->Add("wt_tau_id_highpt_mvadm0_down",tau_sf_mvadm_highpt_mvadm0_down);
        event->Add("wt_tau_id_highpt_mvadm1_down",tau_sf_mvadm_highpt_mvadm1_down);
        event->Add("wt_tau_id_highpt_mvadm2_down",tau_sf_mvadm_highpt_mvadm2_down);
        event->Add("wt_tau_id_highpt_mvadm10_down",tau_sf_mvadm_highpt_mvadm10_down);
        event->Add("wt_tau_id_highpt_mvadm11_down",tau_sf_mvadm_highpt_mvadm11_down);
  
      }
     eventInfo->set_weight("wt_tau_id_sf",tau_sf_1*tau_sf_2);
     if(channel_==channel::tt) event->Add("idisoweight_1", tau_sf_1);
     event->Add("idisoweight_2", tau_sf_2);
    }
    if (do_em_qcd_weights_){
      if(channel_ == channel::em){
       Electron const* elec = dynamic_cast<Electron const*>(dilepton[0]->GetCandidate("lepton1")); 
       Muon const* muon = dynamic_cast<Muon const*>(dilepton[0]->GetCandidate("lepton2")); 
       double qcd_weight=1.0;
       double qcd_weight_up=1.0;
       double qcd_weight_down=1.0;
       double elec_pt = elec->pt(); 
       double muon_pt = muon->pt(); 
       double deltaR = DR(elec,muon);
       if(elec_pt > 100) elec_pt = 99.9;
       if(elec_pt < 10) elec_pt = 10.1;
       if(muon_pt > 100) muon_pt = 99.9;
       if(muon_pt < 10) muon_pt = 10.1;
       double lead_pt = std::max(elec_pt,muon_pt); 
       double trail_pt = std::min(elec_pt,muon_pt); 
       if(era_==era::data_2015){
         if(deltaR < 2){
           qcd_weight = em_qcd_cr1_lt2_->Interpolate(trail_pt,lead_pt);
           qcd_weight_up = em_qcd_cr2_lt2_->Interpolate(trail_pt,lead_pt);
           qcd_weight_down = qcd_weight*qcd_weight/qcd_weight_up;
         } else if (deltaR <=4){
           qcd_weight = em_qcd_cr1_2to4_->Interpolate(trail_pt,lead_pt);
           qcd_weight_up = em_qcd_cr2_2to4_->Interpolate(trail_pt,lead_pt);
           qcd_weight_down = qcd_weight*qcd_weight/qcd_weight_up;
         } else {
           qcd_weight = em_qcd_cr1_gt4_->Interpolate(trail_pt,lead_pt);
           qcd_weight_up = em_qcd_cr2_gt4_->Interpolate(trail_pt,lead_pt);
           qcd_weight_down = qcd_weight*qcd_weight/qcd_weight_up;
         }
       } else if (strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16 || strategy_ == strategy::legacy16 || strategy_ == strategy::cpdecays16 || mc_==mc::mc2017 || mc_==mc::mc2018 || mc_ == mc::mcleg2016){
           std::vector<PFJet*> jets = event->GetPtrVec<PFJet>(jets_label_);
           ic::erase_if(jets,!boost::bind(MinPtMaxEta, _1, 30.0, 4.7));
           double n_jets = (double)jets.size();
           double dR = fabs(ROOT::Math::VectorUtil::DeltaR(elec->vector(),muon->vector()));
           auto args = std::vector<double>{dR,n_jets,elec->pt(),muon->pt()};
           qcd_weight = fns_["em_qcd_osss"]->eval(args.data()); 
           double qcd_extrap_up = fns_["em_qcd_osss_extrap_up"]->eval(args.data())/qcd_weight;
           double qcd_extrap_down = fns_["em_qcd_osss_extrap_down"]->eval(args.data())/qcd_weight;
           event->Add("wt_em_qcd_extrapdown",qcd_extrap_down);
           event->Add("wt_em_qcd_extrapup",qcd_extrap_up);

           double qcd_njets0_unc1_up =   fns_["em_qcd_osss_stat_0jet_unc1_up"]->eval(args.data())/qcd_weight;
           double qcd_njets0_unc1_down =   fns_["em_qcd_osss_stat_0jet_unc1_down"]->eval(args.data())/qcd_weight;
           double qcd_njets0_unc2_up =   fns_["em_qcd_osss_stat_0jet_unc2_up"]->eval(args.data())/qcd_weight;
           double qcd_njets0_unc2_down =   fns_["em_qcd_osss_stat_0jet_unc2_down"]->eval(args.data())/qcd_weight;

           double qcd_njets1_unc1_up =   fns_["em_qcd_osss_stat_1jet_unc1_up"]->eval(args.data())/qcd_weight;
           double qcd_njets1_unc1_down =   fns_["em_qcd_osss_stat_1jet_unc1_down"]->eval(args.data())/qcd_weight;
           double qcd_njets1_unc2_up =   fns_["em_qcd_osss_stat_1jet_unc2_up"]->eval(args.data())/qcd_weight;
           double qcd_njets1_unc2_down =   fns_["em_qcd_osss_stat_1jet_unc2_down"]->eval(args.data())/qcd_weight;

           double qcd_njets2_unc1_up =   fns_["em_qcd_osss_stat_2jet_unc1_up"]->eval(args.data())/qcd_weight;
           double qcd_njets2_unc1_down =   fns_["em_qcd_osss_stat_2jet_unc1_down"]->eval(args.data())/qcd_weight;
           double qcd_njets2_unc2_up =   fns_["em_qcd_osss_stat_2jet_unc2_up"]->eval(args.data())/qcd_weight;
           double qcd_njets2_unc2_down =   fns_["em_qcd_osss_stat_2jet_unc2_down"]->eval(args.data())/qcd_weight;
        
           event->Add("wt_em_qcd_njets0_unc1_up",qcd_njets0_unc1_up);
           event->Add("wt_em_qcd_njets0_unc1_down",qcd_njets0_unc1_down);
           event->Add("wt_em_qcd_njets0_unc2_up",qcd_njets0_unc2_up);
           event->Add("wt_em_qcd_njets0_unc2_down",qcd_njets0_unc2_down);

           event->Add("wt_em_qcd_njets1_unc1_up",qcd_njets1_unc1_up);
           event->Add("wt_em_qcd_njets1_unc1_down",qcd_njets1_unc1_down);
           event->Add("wt_em_qcd_njets1_unc2_up",qcd_njets1_unc2_up);
           event->Add("wt_em_qcd_njets1_unc2_down",qcd_njets1_unc2_down);

           event->Add("wt_em_qcd_njets2_unc1_up",qcd_njets2_unc1_up);
           event->Add("wt_em_qcd_njets2_unc1_down",qcd_njets2_unc1_down);
           event->Add("wt_em_qcd_njets2_unc2_up",qcd_njets2_unc2_up);
           event->Add("wt_em_qcd_njets2_unc2_down",qcd_njets2_unc2_down);

         } 

       event->Add("wt_em_qcd",qcd_weight);
       event->Add("wt_em_qcd_down",qcd_weight_down);
       event->Add("wt_em_qcd_up",qcd_weight_up);
     }
   }
    
    //A derived correction based on a input/MC discrepancy in the subleading b-jet eta in the emu ttbar control region. Used for a cross-check in H->hh analysis.
    if (do_top_jeteta_weights_) {
      std::vector<PFJet*> prebjets = event->GetPtrVec<PFJet>(jets_label_); // Make a copy of the jet collection
      ic::erase_if(prebjets,!boost::bind(MinPtMaxEta, _1, 20.0, 2.4));
      std::sort(prebjets.begin(), prebjets.end(), bind(&PFJet::GetBDiscriminator, _1, btag_label_) > bind(&PFJet::GetBDiscriminator, _2, btag_label_));
      std::vector<PFJet*> prebjets_SF = prebjets;
      // Instead of changing b-tag value in the promote/demote method we look for a map of bools
      // that say whether a jet should pass the WP or not
      if (event->Exists("retag_result")) {
        auto const& retag_result = event->Get<std::map<std::size_t,bool>>("retag_result");
        ic::erase_if(prebjets_SF, !boost::bind(IsReBTagged, _1, retag_result));
      } else {
        ic::erase_if(prebjets_SF, boost::bind(&PFJet::GetBDiscriminator, _1, btag_label_) < 0.679);
      }
      int n_prebjets_SF=prebjets_SF.size();
      double wt=1.00;
      if(n_prebjets_SF>=1) {
        double eta = prebjets[1]->eta();
        if (eta <= -2.0)                           { wt = 1.1897861575; }
        if (eta >= -2.0 && eta < -1.6)             { wt = 1.0639250201; }
        if (eta >= -1.6 && eta < -1.2)             { wt = 1.06317801149; }
        if (eta >= -1.2 && eta < -0.8)             { wt = 0.985871842192; }
        if (eta >= -0.8 && eta < -0.4)             { wt = 0.976539619511; }
        if (eta >= -0.4 && eta < 0.0)              { wt = 0.922588119141; }
        if (eta >= 0.0 && eta < 0.4)               { wt = 1.02200809147; }
        if (eta >= 0.4 && eta < 0.8)               { wt = 0.935068388647; }
        if (eta >= 0.8 && eta < 1.2)               { wt = 0.973272817984; }
        if (eta >= 1.2 && eta < 1.6)               { wt = 1.03197671439; }
        if (eta >= 1.6 && eta < 2.0)               { wt = 1.07689347695; }
        if (eta >= 2.0)                            { wt = 1.13656881923; }
      }
      eventInfo->set_weight("jeteta_weight", wt);
    }
    
    if (do_top_factors_) {
      std::vector<PFJet*> jets = event->GetPtrVec<PFJet>(jets_label_); // Make a copy of the jet collection
      ic::erase_if(jets,!boost::bind(MinPtMaxEta, _1, 30.0, 4.7));
    }
    
    
    if (do_btag_weight_ && mc_!=mc::fall15_76X && mc_!=mc::spring16_80X && mc_ != mc::summer16_80X && mc_!=mc::mc2017 && mc_ != mc::mc2018 && mc_ != mc::mcleg2016) {
      std::vector<PFJet*> jets = event->GetPtrVec<PFJet>(jets_label_); // Make a copy of the jet collection
      ic::erase_if(jets,!boost::bind(MinPtMaxEta, _1, 20.0, 2.4));
      //double no_btag_weight = btag_weight.GetWeight(jets, "CSVM", 0, 0, is_2012_);
      //double inclusive_btag_weight = btag_weight.GetWeight(jets, "CSVM", 1, 99, is_2012_);
      double no_btag_weight = 1.0;
      double inclusive_btag_weight = 1.0;
      BTagWeight::payload set = BTagWeight::payload::ALL2011;
      if (mc_ == mc::summer12_53X) set = BTagWeight::payload::EPS13;
      std::map<std::size_t, bool> retag_result = btag_weight.ReTag(jets, set, BTagWeight::tagger::CSVM, btag_mode_, bfake_mode_);
      event->Add("no_btag_weight", no_btag_weight);
      event->Add("inclusive_btag_weight", inclusive_btag_weight);
      event->Add("retag_result", retag_result);
    }

    if (do_zpt_weight_){
          double zpt = event->Exists("genpT") ? event->Get<double>("genpT") : 0;
          double zmass = event->Exists("genM") ? event->Get<double>("genM") : 0;
          // if want to try with reco vars to check if weight applied properly
          // double zpt = event->Exists("pt_tt") ? event->Get<double>("pt_tt") : 0;
          // double zmass = event->Exists("m_vis") ? event->Get<double>("m_vis") : 0;
      if((mc_ != mc::summer16_80X && mc_ != mc::mc2017 && mc_ != mc::mc2018 && mc_ != mc::mcleg2016)|| strategy_== strategy::smsummer16 || strategy_ == strategy::cpsummer16 || strategy_ == strategy::legacy16 || strategy_ == strategy::cpdecays16){
          double wtzpt = z_pt_mass_hist_->GetBinContent(z_pt_mass_hist_->FindBin(zmass,zpt));
          double wtzpt_down=1.0;
          double wtzpt_up = wtzpt*wtzpt;
          eventInfo->set_weight("wt_zpt",wtzpt);
          event->Add("wt_zpt_up",wtzpt_up/wtzpt);
          event->Add("wt_zpt_down",wtzpt_down/wtzpt);
      } else if(mc_ == mc::summer16_80X && strategy_== strategy::mssmsummer16){
        auto args = std::vector<double>{zmass,zpt};      
        double wtzpt         = fns_["zpt_weight_nom"]->eval(args.data());
        double wtzpt_esup    = fns_["zpt_weight_esup"]->eval(args.data());
        double wtzpt_esdown  = fns_["zpt_weight_esdown"]->eval(args.data());
        double wtzpt_ttup    = fns_["zpt_weight_ttup"]->eval(args.data()); 
        double wtzpt_ttdown  = fns_["zpt_weight_ttdown"]->eval(args.data()); 
        double m400pt0_up    = fns_["zpt_weight_statpt0up"]->eval(args.data()); 
        double m400pt0_down  = fns_["zpt_weight_statpt0down"]->eval(args.data()); 
        double m400pt40_up   = fns_["zpt_weight_statpt40up"]->eval(args.data()); 
        double m400pt40_down = fns_["zpt_weight_statpt40down"]->eval(args.data()); 
        double m400pt80_up   = fns_["zpt_weight_statpt80up"]->eval(args.data());  
        double m400pt80_down = fns_["zpt_weight_statpt80down"]->eval(args.data());
        
        double wtzpt_down=1.0;
        double wtzpt_up = wtzpt*wtzpt;
        eventInfo->set_weight("wt_zpt",wtzpt);
        event->Add("wt_zpt_up",wtzpt_up/wtzpt);
        event->Add("wt_zpt_down",wtzpt_down/wtzpt);
        event->Add("wt_zpt_stat_m400pt0_up"    , m400pt0_up  /wtzpt);
        event->Add("wt_zpt_stat_m400pt40_up"   , m400pt40_up /wtzpt);
        event->Add("wt_zpt_stat_m400pt80_up"   , m400pt80_up /wtzpt);
        event->Add("wt_zpt_stat_m400pt0_down"   , m400pt0_down  /wtzpt);
        event->Add("wt_zpt_stat_m400pt40_down"  , m400pt40_down /wtzpt);
        event->Add("wt_zpt_stat_m400pt80_down"  , m400pt80_down /wtzpt);
        event->Add("wt_zpt_esup"                ,   wtzpt_esup   /wtzpt);
        event->Add("wt_zpt_esdown"              , wtzpt_esdown /wtzpt);
        event->Add("wt_zpt_ttup"                ,   wtzpt_ttup   /wtzpt);
        event->Add("wt_zpt_ttdown"              , wtzpt_ttdown /wtzpt);
      } else if (mc_==mc::mc2017 || mc_ == mc::mc2018 || mc_ == mc::mcleg2016){
        auto args = std::vector<double>{zpt,zmass};  
        double wtzpt = fns_["zpt_weight_nom"]->eval(args.data());
        //double wtzpt = z_pt_mass_hist_->GetBinContent(z_pt_mass_hist_->FindBin(zmass,zpt));
        double wtzpt_down=1.0;
        double wtzpt_up = wtzpt*wtzpt;
        eventInfo->set_weight("wt_zpt",wtzpt);
        event->Add("wt_zpt_up",wtzpt_up/wtzpt);
        event->Add("wt_zpt_down",wtzpt_down/wtzpt);
      } 

    }
    if(mssm_higgspt_file_!="" && do_mssm_higgspt_){

      double pT = -9999;  
      std::vector<ic::GenParticle*> gen_particles = event->GetPtrVec<ic::GenParticle>("genParticles");
      for(unsigned i=0; i<gen_particles.size(); ++i){
        unsigned genID = std::fabs(gen_particles[i]->pdgid());  
        if(genID==36 && gen_particles[i]->statusFlags()[IsLastCopy]) pT = gen_particles[i]->vector().Pt();
      }

      auto args = std::vector<double>{pT};     
      double wt_ggh_t_ = fns_["h_t_ratio"]->eval(args.data());        
      double wt_ggh_b_ = fns_["h_b_ratio"]->eval(args.data());
      double wt_ggh_i_ = fns_["h_i_ratio"]->eval(args.data());
      double wt_ggH_t_ = fns_["H_t_ratio"]->eval(args.data());
      double wt_ggH_b_ = fns_["H_b_ratio"]->eval(args.data());
      double wt_ggH_i_ = fns_["H_i_ratio"]->eval(args.data());
      double wt_ggA_t_ = fns_["A_t_ratio"]->eval(args.data());
      double wt_ggA_b_ = fns_["A_b_ratio"]->eval(args.data());
      double wt_ggA_i_ = fns_["A_i_ratio"]->eval(args.data());
      
      event->Add("wt_ggh_t" ,wt_ggh_t_);
      event->Add("wt_ggh_b" ,wt_ggh_b_);
      event->Add("wt_ggh_i" ,wt_ggh_i_);
      event->Add("wt_ggH_t" ,wt_ggH_t_);
      event->Add("wt_ggH_b" ,wt_ggH_b_);
      event->Add("wt_ggH_i" ,wt_ggH_i_);
      event->Add("wt_ggA_t" ,wt_ggA_t_);
      event->Add("wt_ggA_b" ,wt_ggA_b_);
      event->Add("wt_ggA_i" ,wt_ggA_i_);

    }
    if(do_quarkmass_higgspt_ || do_ps_weights_){
      double HpT=-9999;
      std::vector<ic::GenParticle*> gen_particles = event->GetPtrVec<ic::GenParticle>("genParticles");
      for(unsigned i=0; i<gen_particles.size(); ++i){
        unsigned genID = std::fabs(gen_particles[i]->pdgid());
        if(genID==25 && gen_particles[i]->statusFlags()[IsLastCopy]) HpT = gen_particles[i]->vector().Pt();
      }
      if(do_quarkmass_higgspt_){
        auto args = std::vector<double>{HpT};
        double wt_quarkmass = fns_["ggH_quarkmass_corr"]->eval(args.data());
        double wt_quarkmass_up = fns_["ggH_quarkmass_corr_up"]->eval(args.data());
        double wt_quarkmass_down = fns_["ggH_quarkmass_corr_down"]->eval(args.data());
        eventInfo->set_weight("wt_quarkmass",wt_quarkmass);
        event->Add("wt_quarkmass" ,1./wt_quarkmass);
        event->Add("wt_quarkmass_up" ,wt_quarkmass_up/wt_quarkmass);
        event->Add("wt_quarkmass_down" ,wt_quarkmass_down/wt_quarkmass); 
        double wt_fullquarkmass = fns_["ggH_fullquarkmass_corr"]->eval(args.data());
        event->Add("wt_fullquarkmass" , wt_fullquarkmass/wt_quarkmass);
      }
      if(do_ps_weights_) {
        unsigned ngenjets = event->Get<unsigned>("ngenjets");
        auto args_1 = std::vector<double>{(double)ngenjets};
        auto args_2 = std::vector<double>{(double)ngenjets,HpT};
        double wt_ps_up = fns_["ggH_mg_ps_up"]->eval(args_2.data());
        double wt_ps_down = fns_["ggH_mg_ps_down"]->eval(args_2.data());
        double wt_ue_up = fns_["ggH_mg_ue_up"]->eval(args_1.data());
        double wt_ue_down = fns_["ggH_mg_ue_down"]->eval(args_1.data());
        event->Add("wt_ps_up", wt_ps_up);
        event->Add("wt_ps_down", wt_ps_down);
        event->Add("wt_ue_up", wt_ue_up);
        event->Add("wt_ue_down", wt_ue_down);
      }
    }
    if (do_nnlops_weights_){
      double wt_mg_nnlops = 1.;
      double wt_ph_nnlops = 1.;

      // Get n_jets30 and higgs pt from eventinfo (produced with rivet)
      // https://twiki.cern.ch/twiki/bin/viewauth/CMS/HiggsWG/SignalModelingTools
      unsigned n_jets30 = eventInfo->n_jets30();
      double pt_h = eventInfo->pt_h();
      if(pt_h==0 && n_jets30==0){
        n_jets30 = event->Exists("ngenjets") ? event->Get<unsigned>("ngenjets") : -1;
        std::vector<ic::GenParticle*> gen_particles = event->GetPtrVec<ic::GenParticle>("genParticles");
        for(unsigned i=0; i<gen_particles.size(); ++i){
          unsigned genID = std::fabs(gen_particles[i]->pdgid());
          if(genID==25 && gen_particles[i]->statusFlags()[IsLastCopy]) pt_h = gen_particles[i]->vector().Pt();
        }
      }

      if (n_jets30      == 0) wt_mg_nnlops = ggh_mg_0jet_->Eval(std::min(pt_h, 125.));
      else if (n_jets30 == 1) wt_mg_nnlops = ggh_mg_1jet_->Eval(std::min(pt_h, 625.));
      else if (n_jets30 == 2) wt_mg_nnlops = ggh_mg_2jet_->Eval(std::min(pt_h, 800.));
      else if (n_jets30 >= 3) wt_mg_nnlops = ggh_mg_3jet_->Eval(std::min(pt_h, 925.));
      else wt_mg_nnlops = 1.;

      if (n_jets30      == 0) wt_ph_nnlops = ggh_ph_0jet_->Eval(std::min(pt_h, 125.));
      else if (n_jets30 == 1) wt_ph_nnlops = ggh_ph_1jet_->Eval(std::min(pt_h, 625.));
      else if (n_jets30 == 2) wt_ph_nnlops = ggh_ph_2jet_->Eval(std::min(pt_h, 800.));
      else if (n_jets30 >= 3) wt_ph_nnlops = ggh_ph_3jet_->Eval(std::min(pt_h, 925.));
      else wt_ph_nnlops = 1.;

      event->Add("wt_mg_nnlops", wt_mg_nnlops);
      event->Add("wt_ph_nnlops", wt_ph_nnlops);
    }
    if (do_z_weights_) {
      // these weights are applied for smsummer16 analysis to correct mjj distribution based on Z->mumu data/MC comparrison  
      double wt_z = 1.02;
      double wt_z_mjj = 1.;
      double wt_z_mjj_down = 1.;
      double wt_z_mjj_up = 1.;
      std::vector<PFJet*> jets = event->GetPtrVec<PFJet>(jets_label_);
      ic::erase_if(jets,!boost::bind(MinPtMaxEta, _1, 30.0, 4.7));
      if(jets.size()>=2){
        double mjj = (jets[0]->vector() + jets[1]->vector()).M();
        if(channel_ == channel::et || channel_ == channel::mt || channel_ == channel::em){
          if(mjj>300 && mjj<700){
            wt_z_mjj *= 1.06;
            wt_z_mjj_up *= 1.12;
          } else if (mjj>700 && mjj<1100){
            wt_z_mjj *= 0.98;
            wt_z_mjj_up *= 0.96;
          } else if (mjj>1100){
            wt_z_mjj *= 0.95;
            wt_z_mjj_up *= 0.90;
          }
        }
        if(channel_ == channel::tt){
          if(mjj>300 && mjj<500){
            wt_z_mjj *= 1.02;
            wt_z_mjj_up *= 1.04;
          } else if (mjj>500 && mjj<800){
            wt_z_mjj *= 1.06;
            wt_z_mjj_up *= 1.12;
          } else if (mjj>800){
            wt_z_mjj *= 1.04;
            wt_z_mjj_up *= 1.08;
          }
        }
      }
      eventInfo->set_weight("wt_z",wt_z);
      event->Add("wt_z_mjj", wt_z_mjj);
      event->Add("wt_z_up", wt_z_mjj_up/wt_z_mjj);
      event->Add("wt_z_down", wt_z_mjj_down/wt_z_mjj);
    }

   if (do_tracking_eff_){
       double tracking_wt_1 = 1.0;
       double tracking_wt_2 = 1.0;
     if(channel_ == channel::et){
       Electron const* elec = dynamic_cast<Electron const*>(dilepton[0]->GetCandidate("lepton1"));
       auto args = std::vector<double>{elec->pt(),elec->sc_eta()};
       if(!is_embedded_) tracking_wt_1 *= fns_["e_trk_ratio"]->eval(args.data());
       if(is_embedded_) tracking_wt_1*=fns_["e_trk_embed_ratio"]->eval(args.data());
       tracking_wt_2 = 1.0;
     }
     if(channel_ == channel::mt){
       Muon const* muon = dynamic_cast<Muon const*>(dilepton[0]->GetCandidate("lepton1"));
       auto args = std::vector<double>{muon->eta()};
       tracking_wt_1 *= fns_["m_trk_ratio"]->eval(args.data());
       tracking_wt_2 = 1.0;
     } 
     if(channel_ == channel::em){
       Electron const* elec = dynamic_cast<Electron const*>(dilepton[0]->GetCandidate("lepton1"));
       Muon const* muon = dynamic_cast<Muon const*>(dilepton[0]->GetCandidate("lepton2"));
       auto args = std::vector<double>{elec->pt(),elec->sc_eta()};
       if(!is_embedded_) tracking_wt_1 *= fns_["e_trk_ratio"]->eval(args.data());
       if(is_embedded_) tracking_wt_1*=fns_["e_trk_embed_ratio"]->eval(args.data());
       auto args_2 = std::vector<double>{muon->eta()};
       tracking_wt_2 *= fns_["m_trk_ratio"]->eval(args_2.data());
      }
      if(channel_ == channel::zmm){
       Muon const* muon_1 = dynamic_cast<Muon const*>(dilepton[0]->GetCandidate("lepton1"));
       Muon const* muon_2 = dynamic_cast<Muon const*>(dilepton[0]->GetCandidate("lepton2"));
       auto args = std::vector<double>{muon_1->eta()};
       tracking_wt_1 *= fns_["m_trk_ratio"]->eval(args.data());
       auto args_2 = std::vector<double>{muon_2->eta()};
       tracking_wt_2 *= fns_["m_trk_ratio"]->eval(args_2.data());
      }
      if(channel_ == channel::zee){
       Electron const* elec_1 = dynamic_cast<Electron const*>(dilepton[0]->GetCandidate("lepton1"));
       Electron const* elec_2 = dynamic_cast<Electron const*>(dilepton[0]->GetCandidate("lepton2"));
       auto args = std::vector<double>{elec_1->pt(),elec_1->sc_eta()};
       if(!is_embedded_) tracking_wt_1 *= fns_["e_trk_ratio"]->eval(args.data());
       if(is_embedded_) tracking_wt_1*=fns_["e_trk_embed_ratio"]->eval(args.data());
       auto args_2 = std::vector<double>{elec_2->pt(),elec_2->sc_eta()};
       if(!is_embedded_) tracking_wt_2 *= fns_["e_trk_ratio"]->eval(args_2.data());
       if(is_embedded_) tracking_wt_2*=fns_["e_trk_embed_ratio"]->eval(args_2.data());
      }
      event->Add("trackingweight_1",tracking_wt_1);
      event->Add("trackingweight_2",tracking_wt_2);
      eventInfo->set_weight("wt_tracking_eff",tracking_wt_1*tracking_wt_2);
    }
         

   if (do_trg_weights_) {
     if (channel_ == channel::et) {
       Electron const* elec = dynamic_cast<Electron const*>(dilepton[0]->GetCandidate("lepton1"));
       Tau const* tau = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton2"));
       double e_pt = elec->pt();
       double e_iso = PF03IsolationVal(elec,0.5,0);
       double e_eta = fabs(elec->sc_eta());
       double e_signed_eta = elec->sc_eta();
       if(era_ == era::data_2015 || era_==era::data_2016 || era_ == era::data_2017 
               || era_ == era::data_2018) e_eta = fabs(elec->eta());
       double t_pt = tau->pt();
       double t_signed_eta = tau->eta();
       double t_phi = tau->phi();
       double t_eta = fabs(tau->eta());
       double ele_trg = 1.0;
       double tau_trg = 1.0;
       double ele_trg_mc = 1.0;
       double tau_trg_mc = 1.0;
       if (mc_ == mc::summer12_53X) {
         double emc22ABCD = 1.0;
         double edataRERECO = 1.0;
         if (e_eta < 1.479) {
           emc22ABCD  = Efficiency(e_pt, 21.7243, 0.619015, 0.739301, 1.34903, 1.02594); // MC Reweighted to ABCD
           edataRERECO = Efficiency(e_pt, 22.9704, 1.0258, 1.26889, 1.31024, 1.06409);
         } else {
           emc22ABCD  = Efficiency(e_pt, 22.1217, 1.34054, 1.8885, 1.01855, 4.7241); // MC Reweighted to ABCD
           edataRERECO = Efficiency(e_pt, 21.9816, 1.40993, 0.978597, 2.33144, 0.937552);
         }
         if (era_ == era::data_2012_rereco) {
           ele_trg = edataRERECO;
           ele_trg_mc = emc22ABCD;
         }

         double tdataRERECO = 1.0;
         double tmcRERECO   = 1.0;

         if (t_eta < 1.5) {
           tdataRERECO = Efficiency(t_pt, 18.538229, 0.651562, 0.324869, 13.099048, 0.902365);
           tmcRERECO   = Efficiency(t_pt, 18.605055, 0.264062, 0.139561, 4.792849, 0.915035);
         } else {
           tdataRERECO = Efficiency(t_pt, 18.756548, 0.230732, 0.142859, 3.358497, 0.851919);
           tmcRERECO   = Efficiency(t_pt, 18.557810, 0.280908, 0.119282, 17.749043, 0.865756);

         }
         if (era_ == era::data_2012_rereco) {
           tau_trg = tdataRERECO;
           tau_trg_mc = tmcRERECO;
         }
       } else if (mc_ == mc::fall11_42X) {
         if (fabs(e_eta) < 1.479) {
           double ele15_eb = Efficiency(e_pt, 14.8474, 0.634431, 0.547211, 2.11286, 0.978835);
           double ele18_eb = Efficiency(e_pt, 18.318, 0.811329, 0.938142, 1.85834, 0.927328);
           double ele20_eb = Efficiency(e_pt, 20.244, 0.783044, 0.672118, 2.35819, 0.972123);
           // double ele15_eb = Efficiency(e_pt, 14.8772, 0.311255, 0.221021, 1.87734, 0.986665);
           // double ele18_eb = Efficiency(e_pt, 18.3193, 0.443703, 0.385554, 1.86523, 0.986514);
           // double ele20_eb = Efficiency(e_pt, 20.554, 0.683776, 0.855573, 1.45917, 1.03957);
           ele_trg = (0.41 * ele15_eb) + (0.39 * ele18_eb) + (0.2 * ele20_eb);
           ele_trg_mc = Efficiency(e_pt, 16.9487, 0.162306, 0.0506008, 2.37608, 0.973829);
           // ele_trg_mc = Efficiency(e_pt, 15.1804, 2.43126, 3.85048, 1.72284, 0.998507);
         } else {
           double ele15_ee = Efficiency(e_pt, 16.47, 1.20691, 1.54605, 1.68873, 0.985916);
           double ele18_ee = Efficiency(e_pt, 16.9487, 0.162306, 0.0506008, 2.37608, 0.973829);
           double ele20_ee = Efficiency(e_pt, 22.4559, 1.70818, 0.917021, 3.86463, 0.964978);
           // double ele15_ee = Efficiency(e_pt, 15.6629, 0.759192, 0.47756, 2.02154, 0.998816);
           // double ele18_ee = Efficiency(e_pt, 19.6586, 0.682633, 0.279486, 2.66423, 0.973455);
           // double ele20_ee = Efficiency(e_pt, 23.6386, 1.60775, 1.72093, 1.4131, 1.13962);
           ele_trg = (0.41 * ele15_ee) + (0.39 * ele18_ee) + (0.2 * ele20_ee);
           ele_trg_mc = Efficiency(e_pt, 18.05, 1.66609, 2.3836, 1.49316, 1.01229);
           // ele_trg_mc = Efficiency(e_pt, 16.993, 0.0693958, 0.00695096, 1.9566, 1.00632);
         }
         if (fabs(t_eta) < 1.5) {
           double tau20l_eb = Efficiency(t_pt, 19.3916,  0.996964,  1.70131,   1.38002,   0.903245);
           double tau20m_eb = Efficiency(t_pt, 19.5667,  1.15203 ,  1.68126,   1.40025,   0.848033);
           double tau20t_eb = Efficiency(t_pt, 19.6013,  0.987317,  1.08015,   1.88592,   0.776894);
           tau_trg = (0.25 * tau20l_eb) + (0.59 * tau20m_eb) + (0.16 * tau20t_eb);
           tau_trg_mc = Efficiency(t_pt, 19.468, 0.0615381, 0.0349325, 1.59349, 0.860096);
         } else {
           double tau20l_ee = Efficiency(t_pt, 18.8166,  0.526632,  0.20666,   6.80392,   0.903245);
           double tau20m_ee = Efficiency(t_pt, 18.8476,  0.528963,  0.16717,   3.65814,   0.749759);
           double tau20t_ee = Efficiency(t_pt, 18.8859,  0.271301,  0.128008,  1.50993,   0.825122);
           tau_trg = (0.25 * tau20l_ee) + (0.59 * tau20m_ee) + (0.16 * tau20t_ee);
           tau_trg_mc = Efficiency(t_pt, 19.3862, 0.247148, 0.123187, 2.87108, 0.790894);
         }
       } else if (mc_ == mc::spring15_74X){
         if(e_pt<100){
           ele_trg = et_trig_data_->GetBinContent(et_trig_data_->GetXaxis()->FindBin(e_eta),et_trig_data_->GetYaxis()->FindBin(e_pt));
           ele_trg_mc = et_trig_mc_->GetBinContent(et_trig_mc_->GetXaxis()->FindBin(e_eta),et_trig_mc_->GetYaxis()->FindBin(e_pt));
         } else {
           ele_trg = et_trig_data_->GetBinContent(et_trig_data_->GetXaxis()->FindBin(e_eta),(et_trig_data_->GetYaxis()->FindBin(e_pt)-1));
           ele_trg_mc = et_trig_mc_->GetBinContent(et_trig_mc_->GetXaxis()->FindBin(e_eta),(et_trig_mc_->GetYaxis()->FindBin(e_pt)-1));
         }         
         tau_trg=1;
         tau_trg_mc=1;
       } else if (mc_ == mc::fall15_76X ){
         if(e_pt<150){
           ele_trg = et_trig_data_->GetBinContent(et_trig_data_->GetXaxis()->FindBin(e_eta),et_trig_data_->GetYaxis()->FindBin(e_pt));
           ele_trg_mc = et_trig_mc_->GetBinContent(et_trig_mc_->GetXaxis()->FindBin(e_eta),et_trig_mc_->GetYaxis()->FindBin(e_pt));
         } else {
           ele_trg = et_trig_data_->GetBinContent(et_trig_data_->GetXaxis()->FindBin(e_eta),(et_trig_data_->GetYaxis()->FindBin(e_pt)-1));
           ele_trg_mc = et_trig_mc_->GetBinContent(et_trig_mc_->GetXaxis()->FindBin(e_eta),(et_trig_mc_->GetYaxis()->FindBin(e_pt)-1));
         }         
         tau_trg=1;
         tau_trg_mc=1;

       } else if (mc_ == mc::spring16_80X || mc_ == mc::summer16_80X){
          if(scalefactor_file_==""){ 
             if(do_single_lepton_trg_ && !do_cross_trg_){
               if(e_iso < 0.1){
                 if(e_pt<1000){
                   ele_trg = et_trig_data_->GetBinContent(et_trig_data_->GetXaxis()->FindBin(e_eta),et_trig_data_->GetYaxis()->FindBin(e_pt));
                   //ele_trg_mc = et_trig_mc_->GetBinContent(et_trig_mc_->GetXaxis()->FindBin(e_eta),et_trig_mc_->GetYaxis()->FindBin(e_pt));
                 } else {
                   ele_trg = et_trig_data_->GetBinContent(et_trig_data_->GetXaxis()->FindBin(e_eta),(et_trig_data_->GetYaxis()->FindBin(e_pt)-1));
                   //ele_trg_mc = et_trig_mc_->GetBinContent(et_trig_mc_->GetXaxis()->FindBin(e_eta),(et_trig_mc_->GetYaxis()->FindBin(e_pt)-1));
                 }         
                 tau_trg=1;
                 tau_trg_mc=1;
               }else if(e_iso <0.2){
                 if(e_pt<1000){
                   ele_trg = et_antiiso1_trig_data_->GetBinContent(et_antiiso1_trig_data_->GetXaxis()->FindBin(e_eta),et_antiiso1_trig_data_->GetYaxis()->FindBin(e_pt));
                   //ele_trg_mc = et_trig_antiiso1_mc_->GetBinContent(et_trig_antiiso1_mc_->GetXaxis()->FindBin(e_eta),et_trig_antiiso1_mc_->GetYaxis()->FindBin(e_pt));
                 } else {
                   ele_trg = et_antiiso1_trig_data_->GetBinContent(et_antiiso1_trig_data_->GetXaxis()->FindBin(e_eta),(et_antiiso1_trig_data_->GetYaxis()->FindBin(e_pt)-1));
                   //ele_trg_mc = et_trig_antiiso1_mc_->GetBinContent(et_trig_antiiso1_mc_->GetXaxis()->FindBin(e_eta),(et_trig_antiiso1_mc_->GetYaxis()->FindBin(e_pt)-1));
                 }         
                 tau_trg=1;
                 tau_trg_mc=1;
              } else {//efficiencies only derived for iso<0.5!
                 if(e_pt<1000){
                   ele_trg = et_antiiso2_trig_data_->GetBinContent(et_antiiso2_trig_data_->GetXaxis()->FindBin(e_eta),et_antiiso2_trig_data_->GetYaxis()->FindBin(e_pt));
                   //ele_trg_mc = et_trig_antiiso2_mc_->GetBinContent(et_trig_antiiso2_mc_->GetXaxis()->FindBin(e_eta),et_trig_antiiso2_mc_->GetYaxis()->FindBin(e_pt));
                 } else {
                   ele_trg = et_antiiso2_trig_data_->GetBinContent(et_antiiso2_trig_data_->GetXaxis()->FindBin(e_eta),(et_antiiso2_trig_data_->GetYaxis()->FindBin(e_pt)-1));
                   //ele_trg_mc = et_trig_antiiso2_mc_->GetBinContent(et_trig_antiiso2_mc_->GetXaxis()->FindBin(e_eta),(et_trig_antiiso2_mc_->GetYaxis()->FindBin(e_pt)-1));
                 }         
                 tau_trg=1;
                 tau_trg_mc=1;
               }
             } else if(do_cross_trg_ && !do_single_lepton_trg_){
               if(e_pt<1000){
                 ele_trg = et_xtrig_data_->GetBinContent(et_xtrig_data_->GetXaxis()->FindBin(e_eta),et_xtrig_data_->GetYaxis()->FindBin(e_pt));
                 ele_trg_mc = et_xtrig_mc_->GetBinContent(et_xtrig_mc_->GetXaxis()->FindBin(e_eta),et_xtrig_mc_->GetYaxis()->FindBin(e_pt));
               } else {
                 ele_trg = et_xtrig_data_->GetBinContent(et_xtrig_data_->GetXaxis()->FindBin(e_eta),(et_xtrig_data_->GetYaxis()->FindBin(e_pt)-1));
                 ele_trg_mc = et_xtrig_mc_->GetBinContent(et_xtrig_mc_->GetXaxis()->FindBin(e_eta),(et_xtrig_mc_->GetYaxis()->FindBin(e_pt)-1));
               }         
               tau_trg_mc=1;
               unsigned gm2_ = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_2"));
               if(gm2_ == 5){ //Using tight iso:
                 tau_trg       = Efficiency(t_pt, 21.1744, 0.773395, 0.705463, 1.65358, 1.0000);
               } else {
                 tau_trg       = Efficiency(t_pt, 20.7216, 1.86861, 1.79281, 1.56784, 9.86642e-01);
               } 
             } else if(do_cross_trg_ && do_single_lepton_trg_){
               double ele_sgl_trg=1.0;
               double ele_sgl_trg_mc=1.0;
               double ele_cond_trg=1.0;
               double ele_cond_trg_mc=1.0;
               if(e_pt<1000){
                 ele_trg = et_xtrig_data_->GetBinContent(et_xtrig_data_->GetXaxis()->FindBin(e_eta),et_xtrig_data_->GetYaxis()->FindBin(e_pt));
                 ele_trg_mc = et_xtrig_mc_->GetBinContent(et_xtrig_mc_->GetXaxis()->FindBin(e_eta),et_xtrig_mc_->GetYaxis()->FindBin(e_pt));
                 ele_sgl_trg = et_trig_data_->GetBinContent(et_trig_data_->GetXaxis()->FindBin(e_eta),et_trig_data_->GetYaxis()->FindBin(e_pt));
                 ele_sgl_trg_mc = et_trig_mc_->GetBinContent(et_trig_mc_->GetXaxis()->FindBin(e_eta),et_trig_mc_->GetYaxis()->FindBin(e_pt));
                 ele_cond_trg = et_conditional_data_->GetBinContent(et_conditional_data_->GetXaxis()->FindBin(e_eta),et_conditional_data_->GetYaxis()->FindBin(e_pt));
                 ele_cond_trg_mc = et_conditional_mc_->GetBinContent(et_conditional_mc_->GetXaxis()->FindBin(e_eta),et_conditional_mc_->GetYaxis()->FindBin(e_pt));
               } else {
                 ele_trg = et_xtrig_data_->GetBinContent(et_xtrig_data_->GetXaxis()->FindBin(e_eta),(et_xtrig_data_->GetYaxis()->FindBin(e_pt)-1));
                 ele_trg_mc = et_xtrig_mc_->GetBinContent(et_xtrig_mc_->GetXaxis()->FindBin(e_eta),(et_xtrig_mc_->GetYaxis()->FindBin(e_pt)-1));
                 ele_sgl_trg = et_trig_data_->GetBinContent(et_trig_data_->GetXaxis()->FindBin(e_eta),(et_trig_data_->GetYaxis()->FindBin(e_pt)-1));
                 ele_sgl_trg_mc = et_trig_mc_->GetBinContent(et_trig_mc_->GetXaxis()->FindBin(e_eta),(et_trig_mc_->GetYaxis()->FindBin(e_pt)-1));
                 ele_cond_trg = et_conditional_data_->GetBinContent(et_conditional_data_->GetXaxis()->FindBin(e_eta),(et_conditional_data_->GetYaxis()->FindBin(e_pt)-1));
                 ele_cond_trg_mc = et_conditional_mc_->GetBinContent(et_conditional_mc_->GetXaxis()->FindBin(e_eta),(et_conditional_mc_->GetYaxis()->FindBin(e_pt)-1));
               }         
               tau_trg_mc=1;
               unsigned gm2_ = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_2"));
               if(gm2_ == 5){ //Using tight iso:
                 tau_trg       = Efficiency(t_pt, 21.1744, 0.773395, 0.705463, 1.65358, 1.0000);
               } else {
                 tau_trg       = Efficiency(t_pt, 20.7216, 1.86861, 1.79281, 1.56784, 9.86642e-01);
               } 
               ele_trg = ele_sgl_trg+tau_trg*ele_trg-tau_trg*ele_cond_trg*ele_trg; //Conditional probability: P(single)+P(crosstrig)-P(crosstrig)*P(singlelept|muleg crosstrig)
               ele_trg_mc = ele_sgl_trg_mc+tau_trg_mc*ele_trg_mc-tau_trg_mc*ele_cond_trg_mc*ele_trg_mc; //Conditional probability: P(single)+P(crosstrig)-P(crosstrig)*P(singlelept|muleg crosstrig)
               tau_trg = 1.0; //In this case full weight is stored as ele_trg weight 
             } 

           } else {
             if(!do_cross_trg_ && do_single_lepton_trg_){
                 tau_trg = 1;
                 tau_trg_mc=1;
                 auto args_1 = std::vector<double>{e_pt,e_signed_eta,e_iso};
                 auto args_desy = std::vector<double>{e_pt,e_signed_eta};
                 if(mc_ == mc::summer16_80X && strategy_ != strategy::smsummer16 && strategy_ != strategy::cpsummer16 &&  strategy_ != strategy::legacy16 && strategy_ != strategy::cpdecays16){
                   if(e_iso<0.1){
                     ele_trg = fns_["e_trgEle25eta2p1WPTight_desy_data"]->eval(args_desy.data());
                   } else ele_trg = fns_["e_trg_binned_data"]->eval(args_1.data());
                   ele_trg_mc=1;
                   if(mc_ == mc::summer16_80X){
                     ele_trg = fns_["e_trg_binned_data"]->eval(args_1.data());  
                     ele_trg_mc = fns_["e_trg_binned_mc"]->eval(args_1.data());
                   }
                 } else if (strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16 || strategy_ == strategy::legacy16 || strategy_ == strategy::cpdecays16){
                   if(is_embedded_){
                     ele_trg = fns_["e_trg_binned_data"]->eval(args_1.data());    
                     ele_trg_mc = fns_["e_trg_binned_mc"]->eval(args_1.data());
                   } else {
                     ele_trg = fns_["e_trgEle25eta2p1WPTight_desy_data"]->eval(args_desy.data());  
                     ele_trg_mc = fns_["e_trgEle25eta2p1WPTight_desy_mc"]->eval(args_desy.data());
                     if(e_iso>0.1){
                       ele_trg = fns_["e_trgEle25eta2p1WPTight_aiso0p1to0p3_desy_data"]->eval(args_desy.data());  
                       ele_trg_mc = fns_["e_trgEle25eta2p1WPTight_aiso0p1to0p3_desy_mc"]->eval(args_desy.data());    
                     }
                   }
                 } 
             } else {
                 std::cout << "Cross trigger not currently supported! Setting trigger efficiencies to 1" << std::endl;
                 tau_trg=1;
                 tau_trg_mc=1;
                 ele_trg=1;
                 ele_trg_mc=1;
             }
          }
       } else if (mc_ == mc::mc2017 || mc_ == mc::mc2018 || mc_ == mc::mcleg2016) {
            // add here for e tau cross trg
            e_iso = PF03EAIsolationVal(elec, eventInfo->jet_rho());
            auto args_1 = std::vector<double>{e_pt,e_eta,e_iso};
            ele_trg = fns_["e_trg_binned_data"]->eval(args_1.data());
            if(!is_embedded_) ele_trg_mc = fns_["e_trg_binned_mc"]->eval(args_1.data());
            else ele_trg_mc = fns_["e_trg_binned_embed"]->eval(args_1.data());


            double single_e_sf = ele_trg_mc>0 ? ele_trg / ele_trg_mc : 0.;
            double t_dm = tau->decay_mode();
            auto args_2 = std::vector<double>{e_pt,e_eta};  
            auto args_3 = std::vector<double>{t_pt,t_signed_eta,t_phi,t_dm};  
            auto args_4 = std::vector<double>{t_pt,t_dm};

            double t_mvadm = tau->GetTauID("MVADM2017v1");
            auto args_mvadm = std::vector<double>{t_pt,t_mvadm};

            double ele_xtrg = 1.;
            double ele_xtrg_mc=1.;
            double tau_trg_ic=1., tau_trg_mvadm=1., tau_trg_ic_data=1., tau_trg_mvadm_data=1., tau_trg_data=1.;

            if(mc_ == mc::mc2017 || mc_ == mc::mc2018) {

              ele_xtrg = fns_["e_crosstrg_data"]->eval(args_1.data());
              if(is_embedded_) ele_xtrg_mc = fns_["e_crosstrg_embed"]->eval(args_1.data());
              else ele_xtrg_mc = fns_["e_crosstrg_mc"]->eval(args_1.data());
              
              if(is_embedded_) {
                tau_trg_mc = 1.;
                tau_trg = fns_["t_trg_30_embed_ratio"]->eval(args_3.data());
                tau_trg_data = fns_["t_trg_30_embed_data"]->eval(args_3.data());
                tau_trg_ic = fns_["t_trg_30_ic_embed_ratio"]->eval(args_4.data());
                tau_trg_mvadm = fns_["t_trg_30_ic_mvadm_embed_ratio"]->eval(args_mvadm.data());
                tau_trg_ic_data = fns_["t_trg_30_ic_embed_data"]->eval(args_4.data());
                tau_trg_mvadm_data = fns_["t_trg_30_ic_mvadm_embed_data"]->eval(args_mvadm.data());
              }
              else {
                tau_trg_mc = 1.;
                tau_trg = fns_["t_trg_30_ratio"]->eval(args_4.data());
                tau_trg_ic = fns_["t_trg_30_ic_ratio"]->eval(args_4.data());
                tau_trg_mvadm = fns_["t_trg_30_ic_mvadm_ratio"]->eval(args_mvadm.data());

              }

            }

            double e_high_pt_cut=28.;
            if(mc_ == mc::mc2018) e_high_pt_cut=33;

            double xtrg_et_sf = (ele_xtrg_mc*tau_trg_mc) > 0 ? (ele_xtrg*tau_trg)/(ele_xtrg_mc*tau_trg_mc) : 0.0;

            double xtrg_OR_sf = (e_pt >= e_high_pt_cut) ? single_e_sf : xtrg_et_sf;

            // if trigger is not applied in the MC then set the SF to the efficiency
            if(!trg_applied_in_mc_) {
              xtrg_et_sf = ele_xtrg*tau_trg;
              xtrg_OR_sf = ele_trg*(1-tau_trg) + ele_xtrg*tau_trg;
              single_e_sf = ele_trg;
            }


            if(mc_ == mc::mc2017 && is_embedded_ && e_pt<40 && fabs(e_eta)>1.479){
              // electron triggers in this eta/pT region don't work properly for the embedding in 2017 so set the SF to the data efficiency and have all events pass the trigger in HTTTriggerFilter - this works for the 2018 but needs to be checked for the 2016 legacy!
              single_e_sf = ele_trg;
              xtrg_et_sf = ele_xtrg*tau_trg_data;
              xtrg_OR_sf = (e_pt >= e_high_pt_cut) ? single_e_sf : xtrg_et_sf;
              tau_trg_ic = tau_trg_ic_data; 
              tau_trg_mvadm = tau_trg_mvadm_data; 
            }
     
            // for 2016 we only use the single electron trigger
            if(mc_ == mc::mcleg2016) {
              xtrg_et_sf=0.;
              xtrg_OR_sf=single_e_sf;
            }

            if(mc_ != mc::mcleg2016) {
              tau_trg_ic = (tau_trg==0) ? tau_trg_ic : tau_trg_ic/tau_trg;
              tau_trg_mvadm = (tau_trg==0) ? tau_trg_mvadm : tau_trg_mvadm/tau_trg;
              tau_trg_ic = (e_pt>=e_high_pt_cut) ? 1. : tau_trg_ic;
              tau_trg_mvadm = (e_pt>=e_high_pt_cut) ? 1. : tau_trg_mvadm;
              event->Add("wt_tau_trg_ic",tau_trg_ic);
              event->Add("wt_tau_trg_mvadm",tau_trg_mvadm);
            } else {
              event->Add("wt_tau_trg_ic", 1.);
              event->Add("wt_tau_trg_mvadm", 1.);
            }

            eventInfo->set_weight("trigger", xtrg_OR_sf, false);

            double trigweight_up=1., trigweight_down=1.;
            double tau_trg_dm0_up=1.;
            double tau_trg_dm1_up=1.;
            double tau_trg_dm10_up=1.;
            double tau_trg_dm11_up=1.;
            double tau_trg_dm0_down=1.;
            double tau_trg_dm1_down=1.;
            double tau_trg_dm10_down=1.;
            double tau_trg_dm11_down=1.;

            double tau_trg_mvadm0_up=1;
            double tau_trg_mvadm1_up=1;
            double tau_trg_mvadm2_up=1;
            double tau_trg_mvadm10_up=1;
            double tau_trg_mvadm11_up=1;
            double tau_trg_mvadm0_down=1;
            double tau_trg_mvadm1_down=1;
            double tau_trg_mvadm2_down=1;
            double tau_trg_mvadm10_down=1;
            double tau_trg_mvadm11_down=1;


            if(mc_ == mc::mc2017 || mc_ == mc::mc2018) {

              if(e_pt<e_high_pt_cut) { // if this isn't true then we are using the single lepton trigger and therefore the weight = 1
                if(!is_embedded_) {
                  if(fns_["t_trg_30_ratio"]->eval(args_4.data())>0){
                    tau_trg_dm0_up=fns_["t_trg_30_ratio_dm0_up"]->eval(args_4.data())/fns_["t_trg_30_ratio"]->eval(args_4.data());
                    tau_trg_dm1_up=fns_["t_trg_30_ratio_dm1_up"]->eval(args_4.data())/fns_["t_trg_30_ratio"]->eval(args_4.data());
                    tau_trg_dm10_up=fns_["t_trg_30_ratio_dm10_up"]->eval(args_4.data())/fns_["t_trg_30_ratio"]->eval(args_4.data());
                    tau_trg_dm11_up=fns_["t_trg_30_ratio_dm11_up"]->eval(args_4.data())/fns_["t_trg_30_ratio"]->eval(args_4.data());
                    tau_trg_dm0_down=fns_["t_trg_30_ratio_dm0_down"]->eval(args_4.data())/fns_["t_trg_30_ratio"]->eval(args_4.data());
                    tau_trg_dm1_down=fns_["t_trg_30_ratio_dm1_down"]->eval(args_4.data())/fns_["t_trg_30_ratio"]->eval(args_4.data());
                    tau_trg_dm10_down=fns_["t_trg_30_ratio_dm10_down"]->eval(args_4.data())/fns_["t_trg_30_ratio"]->eval(args_4.data());
                    tau_trg_dm11_down=fns_["t_trg_30_ratio_dm11_down"]->eval(args_4.data())/fns_["t_trg_30_ratio"]->eval(args_4.data());
                  }
                  if(fns_["t_trg_30_ic_mvadm_ratio"]->eval(args_mvadm.data())>0){
                    tau_trg_mvadm0_up   =fns_["t_trg_30_ic_mvadm_ratio_mvadm0_up"]->eval(args_mvadm.data())   /fns_["t_trg_30_ic_mvadm_ratio"]->eval(args_mvadm.data());
                    tau_trg_mvadm1_up   =fns_["t_trg_30_ic_mvadm_ratio_mvadm1_up"]->eval(args_mvadm.data())   /fns_["t_trg_30_ic_mvadm_ratio"]->eval(args_mvadm.data());
                    tau_trg_mvadm2_up   =fns_["t_trg_30_ic_mvadm_ratio_mvadm2_up"]->eval(args_mvadm.data())   /fns_["t_trg_30_ic_mvadm_ratio"]->eval(args_mvadm.data());
                    tau_trg_mvadm10_up  =fns_["t_trg_30_ic_mvadm_ratio_mvadm10_up"]->eval(args_mvadm.data())  /fns_["t_trg_30_ic_mvadm_ratio"]->eval(args_mvadm.data());
                    tau_trg_mvadm11_up  =fns_["t_trg_30_ic_mvadm_ratio_mvadm11_up"]->eval(args_mvadm.data())  /fns_["t_trg_30_ic_mvadm_ratio"]->eval(args_mvadm.data());
                    tau_trg_mvadm0_down =fns_["t_trg_30_ic_mvadm_ratio_mvadm0_down"]->eval(args_mvadm.data()) /fns_["t_trg_30_ic_mvadm_ratio"]->eval(args_mvadm.data());
                    tau_trg_mvadm1_down =fns_["t_trg_30_ic_mvadm_ratio_mvadm1_down"]->eval(args_mvadm.data()) /fns_["t_trg_30_ic_mvadm_ratio"]->eval(args_mvadm.data());
                    tau_trg_mvadm2_down =fns_["t_trg_30_ic_mvadm_ratio_mvadm2_down"]->eval(args_mvadm.data()) /fns_["t_trg_30_ic_mvadm_ratio"]->eval(args_mvadm.data());
                    tau_trg_mvadm10_down=fns_["t_trg_30_ic_mvadm_ratio_mvadm10_down"]->eval(args_mvadm.data())/fns_["t_trg_30_ic_mvadm_ratio"]->eval(args_mvadm.data());
                    tau_trg_mvadm11_down=fns_["t_trg_30_ic_mvadm_ratio_mvadm11_down"]->eval(args_mvadm.data())/fns_["t_trg_30_ic_mvadm_ratio"]->eval(args_mvadm.data());
                  }
  
                } else {
                  if(fns_["t_trg_30_embed_ratio"]->eval(args_3.data())>0) {
                    tau_trg_dm0_up=fns_["t_trg_30_embed_ratio_dm0_up"]->eval(args_3.data())/fns_["t_trg_30_embed_ratio"]->eval(args_3.data());
                    tau_trg_dm1_up=fns_["t_trg_30_embed_ratio_dm1_up"]->eval(args_3.data())/fns_["t_trg_30_embed_ratio"]->eval(args_3.data());
                    tau_trg_dm10_up=fns_["t_trg_30_embed_ratio_dm10_up"]->eval(args_3.data())/fns_["t_trg_30_embed_ratio"]->eval(args_3.data());
                    tau_trg_dm11_up=fns_["t_trg_30_embed_ratio_dm11_up"]->eval(args_3.data())/fns_["t_trg_30_embed_ratio"]->eval(args_3.data());
                    tau_trg_dm0_down=fns_["t_trg_30_embed_ratio_dm0_down"]->eval(args_3.data())/fns_["t_trg_30_embed_ratio"]->eval(args_3.data());
                    tau_trg_dm1_down=fns_["t_trg_30_embed_ratio_dm1_down"]->eval(args_3.data())/fns_["t_trg_30_embed_ratio"]->eval(args_3.data());
                    tau_trg_dm10_down=fns_["t_trg_30_embed_ratio_dm10_down"]->eval(args_3.data())/fns_["t_trg_30_embed_ratio"]->eval(args_3.data());
                    tau_trg_dm11_down=fns_["t_trg_30_embed_ratio_dm11_down"]->eval(args_3.data())/fns_["t_trg_30_embed_ratio"]->eval(args_3.data());
                  }
                  if(fns_["t_trg_30_ic_mvadm_embed_ratio"]->eval(args_mvadm.data())>0){
                    tau_trg_mvadm0_up   =fns_["t_trg_30_ic_mvadm_embed_ratio_mvadm0_up"]->eval(args_mvadm.data())   /fns_["t_trg_30_ic_mvadm_embed_ratio"]->eval(args_mvadm.data());
                    tau_trg_mvadm1_up   =fns_["t_trg_30_ic_mvadm_embed_ratio_mvadm1_up"]->eval(args_mvadm.data())   /fns_["t_trg_30_ic_mvadm_embed_ratio"]->eval(args_mvadm.data());
                    tau_trg_mvadm2_up   =fns_["t_trg_30_ic_mvadm_embed_ratio_mvadm2_up"]->eval(args_mvadm.data())   /fns_["t_trg_30_ic_mvadm_embed_ratio"]->eval(args_mvadm.data());
                    tau_trg_mvadm10_up  =fns_["t_trg_30_ic_mvadm_embed_ratio_mvadm10_up"]->eval(args_mvadm.data())  /fns_["t_trg_30_ic_mvadm_embed_ratio"]->eval(args_mvadm.data());
                    tau_trg_mvadm11_up  =fns_["t_trg_30_ic_mvadm_embed_ratio_mvadm11_up"]->eval(args_mvadm.data())  /fns_["t_trg_30_ic_mvadm_embed_ratio"]->eval(args_mvadm.data());
                    tau_trg_mvadm0_down =fns_["t_trg_30_ic_mvadm_embed_ratio_mvadm0_down"]->eval(args_mvadm.data()) /fns_["t_trg_30_ic_mvadm_embed_ratio"]->eval(args_mvadm.data());
                    tau_trg_mvadm1_down =fns_["t_trg_30_ic_mvadm_embed_ratio_mvadm1_down"]->eval(args_mvadm.data()) /fns_["t_trg_30_ic_mvadm_embed_ratio"]->eval(args_mvadm.data());
                    tau_trg_mvadm2_down =fns_["t_trg_30_ic_mvadm_embed_ratio_mvadm2_down"]->eval(args_mvadm.data()) /fns_["t_trg_30_ic_mvadm_embed_ratio"]->eval(args_mvadm.data());
                    tau_trg_mvadm10_down=fns_["t_trg_30_ic_mvadm_embed_ratio_mvadm10_down"]->eval(args_mvadm.data())/fns_["t_trg_30_ic_mvadm_embed_ratio"]->eval(args_mvadm.data());
                    tau_trg_mvadm11_down=fns_["t_trg_30_ic_mvadm_embed_ratio_mvadm11_down"]->eval(args_mvadm.data())/fns_["t_trg_30_ic_mvadm_embed_ratio"]->eval(args_mvadm.data());
                  }  
                }
              }

              tau_trg_dm0_up = std::isnan(tau_trg_dm0_up) ? 0 : tau_trg_dm0_up;
              tau_trg_dm0_down = std::isnan(tau_trg_dm0_down) ? 0 : tau_trg_dm0_down;
              tau_trg_dm1_up = std::isnan(tau_trg_dm1_up) ? 0 : tau_trg_dm1_up;
              tau_trg_dm1_down = std::isnan(tau_trg_dm1_down) ? 0 : tau_trg_dm1_down;
              tau_trg_dm10_up = std::isnan(tau_trg_dm10_up) ? 0 : tau_trg_dm10_up;
              tau_trg_dm10_down = std::isnan(tau_trg_dm10_down) ? 0 : tau_trg_dm10_down;
              tau_trg_dm11_up = std::isnan(tau_trg_dm11_up) ? 0 : tau_trg_dm11_up;
              tau_trg_dm11_down = std::isnan(tau_trg_dm11_down) ? 0 : tau_trg_dm11_down;

              tau_trg_mvadm0_up    = std::isnan(tau_trg_mvadm0_up)    ? 0 : tau_trg_mvadm0_up;
              tau_trg_mvadm0_down  = std::isnan(tau_trg_mvadm0_down)  ? 0 : tau_trg_mvadm0_down;
              tau_trg_mvadm1_up    = std::isnan(tau_trg_mvadm1_up)    ? 0 : tau_trg_mvadm1_up;
              tau_trg_mvadm1_down  = std::isnan(tau_trg_mvadm1_down)  ? 0 : tau_trg_mvadm1_down;
              tau_trg_mvadm2_up    = std::isnan(tau_trg_mvadm2_up)    ? 0 : tau_trg_mvadm2_up;
              tau_trg_mvadm2_down  = std::isnan(tau_trg_mvadm2_down)  ? 0 : tau_trg_mvadm2_down;
              tau_trg_mvadm10_up   = std::isnan(tau_trg_mvadm10_up)   ? 0 : tau_trg_mvadm10_up;
              tau_trg_mvadm10_down = std::isnan(tau_trg_mvadm10_down) ? 0 : tau_trg_mvadm10_down;
              tau_trg_mvadm11_up   = std::isnan(tau_trg_mvadm11_up)   ? 0 : tau_trg_mvadm11_up;
              tau_trg_mvadm11_down = std::isnan(tau_trg_mvadm11_down) ? 0 : tau_trg_mvadm11_down;

              event->Add("wt_tau_trg_dm0_up",tau_trg_dm0_up);
              event->Add("wt_tau_trg_dm1_up",tau_trg_dm1_up);
              event->Add("wt_tau_trg_dm10_up",tau_trg_dm10_up);
              event->Add("wt_tau_trg_dm11_up",tau_trg_dm11_up);
              event->Add("wt_tau_trg_dm0_down",tau_trg_dm0_down);
              event->Add("wt_tau_trg_dm1_down",tau_trg_dm1_down);
              event->Add("wt_tau_trg_dm10_down",tau_trg_dm10_down);
              event->Add("wt_tau_trg_dm11_down",tau_trg_dm11_down);

              event->Add("wt_tau_trg_mvadm0_up"   ,tau_trg_mvadm0_up);
              event->Add("wt_tau_trg_mvadm1_up"   ,tau_trg_mvadm1_up);
              event->Add("wt_tau_trg_mvadm2_up"   ,tau_trg_mvadm2_up);
              event->Add("wt_tau_trg_mvadm10_up"  ,tau_trg_mvadm10_up);
              event->Add("wt_tau_trg_mvadm11_up"  ,tau_trg_mvadm11_up);
              event->Add("wt_tau_trg_mvadm0_down" ,tau_trg_mvadm0_down);
              event->Add("wt_tau_trg_mvadm1_down" ,tau_trg_mvadm1_down);
              event->Add("wt_tau_trg_mvadm2_down" ,tau_trg_mvadm2_down);
              event->Add("wt_tau_trg_mvadm10_down",tau_trg_mvadm10_down);
              event->Add("wt_tau_trg_mvadm11_down",tau_trg_mvadm11_down);

            }

            event->Add("trigweight_up", trigweight_up);
            event->Add("trigweight_down", trigweight_down);
 
            if(mc_ == mc::mc2017 ){
              // in 2017 these is an additional factor recommended by the EGammas POG to correct electron triggers
              xtrg_OR_sf*=0.991;
              single_e_sf*=0.991;
              xtrg_et_sf*=0.991;
            }
 
            // have xtrg OR as default but save others to check 
            event->Add("single_l_sf", xtrg_OR_sf==0 ? single_e_sf : single_e_sf/xtrg_OR_sf );
            event->Add("xtrg_sf", xtrg_OR_sf==0 ? xtrg_et_sf : xtrg_et_sf);


            ele_trg = xtrg_OR_sf;
            ele_trg_mc = 1.0;

            tau_trg = 1.0;
            tau_trg_mc = 1.0; 
       }

       if (trg_applied_in_mc_) {
         ele_trg = ele_trg / ele_trg_mc;
         tau_trg = tau_trg / tau_trg_mc;
       }
       if(ele_trg>2) ele_trg=2;
       weight *= (ele_trg * tau_trg);
       event->Add("trigweight_1", ele_trg);
       event->Add("trigweight_2", tau_trg);
     } else if (channel_ == channel::mt) {
       Muon const* muon = dynamic_cast<Muon const*>(dilepton[0]->GetCandidate("lepton1"));
       double pt = muon->pt();
       double m_eta = fabs(muon->eta());
       double m_signed_eta = muon->eta();
       double m_iso = PF04IsolationVal(muon, 0.5, 0);
       double m_a_eta = muon->eta();
       Tau const* tau = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton2"));
       double t_pt = tau->pt();
       double t_eta = fabs(tau->eta());
       double t_signed_eta = tau->eta();
       double t_phi = tau->phi();
       double mu_trg = 1.0;
       double tau_trg = 1.0;
       double mu_trg_mc = 1.0;
       double tau_trg_mc = 1.0;
       if (mc_ == mc::summer12_53X) {
         double muRERECO = 1.0;
         double mcRERECO = 1.0;
         if (m_a_eta >= 0 && m_a_eta < 0.8) {
           muRERECO  = Efficiency(pt, 17.313, 0.662731, 1.3412, 1.05778, 1.26624);
           mcRERECO  = Efficiency(pt, 15.9289, 0.0271317, 0.00448573, 1.92101, 0.978625);
         } else if (m_a_eta >= 0.8 && m_a_eta < 1.2) {
           muRERECO  = Efficiency(pt, 16.9966, 0.550532, 0.807863, 1.55402, 0.885134);
           mcRERECO  = Efficiency(pt, 16.5678, 0.328333, 0.354533, 1.67085, 0.916992);
         } else if (m_a_eta >= 1.2) {
           muRERECO  = Efficiency(pt, 15.9962, 0.000106195, 4.95058e-08, 1.9991, 0.851294);
           mcRERECO  = Efficiency(pt, 15.997, 7.90069e-05, 4.40036e-08, 1.66272, 0.884502);
         } else if (m_a_eta >= -0.8 && m_a_eta < 0) {
           muRERECO  = Efficiency(pt, 16.4307, 0.226312, 0.265553, 1.55756, 0.974462);
           mcRERECO  = Efficiency(pt, 15.9556, 0.0236127, 0.00589832, 1.75409, 0.981338);
         } else if (m_a_eta >= -1.2 && m_a_eta < -0.8) {
           muRERECO  = Efficiency(pt, 17.3974, 0.804001, 1.47145, 1.24295, 0.928198);
           mcRERECO  = Efficiency(pt, 17.3135, 0.747636, 1.21803, 1.40611, 0.934983);
         } else if (m_a_eta < -1.2) {
           muRERECO  = Efficiency(pt, 15.9977, 7.64004e-05, 6.4951e-08, 1.57403, 0.865325);
           mcRERECO  = Efficiency(pt, 16.0051, 2.45144e-05, 4.3335e-09, 1.66134, 0.87045);
         }
         if (era_ == era::data_2012_rereco) {
           mu_trg = muRERECO;
           mu_trg_mc = mcRERECO;
         }

         double tdataRERECO   = 1.0;
         double tmcRERECO     = 1.0;

         if (fabs(t_eta) < 1.5) {
           tdataRERECO  = Efficiency(t_pt, 18.604910, 0.276042, 0.137039, 2.698437, 0.940721);
           tmcRERECO    = Efficiency(t_pt, 18.532997, 1.027880, 2.262950, 1.003322, 5.297292);
         } else {
           tdataRERECO  = Efficiency(t_pt, 18.701715, 0.216523, 0.148111, 2.245081, 0.895320);
           tmcRERECO    = Efficiency(t_pt, 18.212782, 0.338119, 0.122828, 12.577926, 0.893975);

         }
         if (era_ == era::data_2012_rereco) {
           tau_trg = tdataRERECO;
           tau_trg_mc = tmcRERECO;
         }
       } else if (mc_ == mc::fall11_42X) {
         if (m_eta < 0.8) {
           double mu12 = 0.920;
           double mu15 = 0.917;
           double mu15_2p1 = Efficiency(pt, 15.9877, 2.90938e-07, 2.63922e-11, 5.81194, 0.906943);
           mu_trg = (0.034 * mu12) + (0.368 * mu15) + (0.598 * mu15_2p1);
           mu_trg_mc = 0.923;
         } else if (m_eta >= 0.8 && m_eta < 1.2) {
           double mu12 = 0.868;
           double mu15 = 0.871;
           double mu15_2p1 = Efficiency(pt, 15.9995, 1.35931e-07, 7.88264e-11, 4.60253, 0.855461);
           mu_trg = (0.034 * mu12) + (0.368 * mu15) + (0.598 * mu15_2p1);
           mu_trg_mc = 0.879;
         } else {
           double mu12 = 0.845;
           double mu15 = 0.864;
           double mu15_2p1 = Efficiency(pt, 15.9084, 2.27242e-12, 8.77174e-14, 1.00241, 12.9909);
           mu_trg = (0.034 * mu12) + (0.368 * mu15) + (0.598 * mu15_2p1);
           mu_trg_mc = 0.839;
         }
         // if (fabs(m_eta) < 1.4) {
         //   double mu1215_eb = 0.901;
         //   double mu15_2p1_eb = Efficiency(pt, 15.06,  0.55278,   1.34236,   1.003,   3.36767);
         //   mu_trg = (0.402 * mu1215_eb) + (0.598 * mu15_2p1_eb);
         //   mu_trg_mc = 0.917;
         // } else {
         //   double mu1215_ee = 0.863;
         //   double mu15_2p1_ee = Efficiency(pt, 15.03175,   0.866114,  1.25009,   1.63711,   0.844906);
         //   mu_trg = (0.402 * mu1215_ee) + (0.598 * mu15_2p1_ee);
         //   mu_trg_mc = 0.836;
         // }

         if (fabs(t_eta) < 1.5) {
           double tau10l_eb = Efficiency(t_pt, 13.6046,   1.66291,   1.71551,   141.929,   0.910686);
           double tau15l_eb = Efficiency(t_pt, 13.9694,   0.084835,  0.057743,  1.50674,   0.984976);
           double tau20l_eb = Efficiency(t_pt, 19.2102,   1.26519,   2.48994,   1.04699,  1.3492);
           tau_trg = (0.043 * tau10l_eb) + (0.359 * tau15l_eb) + (0.598 * tau20l_eb);
           tau_trg_mc = Efficiency(t_pt, 14.4601, 0.0485272, 0.03849, 1.48324, 0.965257);
         } else {
           double tau10l_ee = Efficiency(t_pt, -0.392211,   7.90467,   5.48228,   134.599,   0.925858);
           double tau15l_ee = Efficiency(t_pt, 14.435,  1.34952,   2.43996,   1.03631,   1.79081);
           double tau20l_ee = Efficiency(t_pt, 19.2438,   1.37298,   1.76448,   1.73935,   0.901291);
           tau_trg = (0.043 * tau10l_ee) + (0.359 * tau15l_ee) + (0.598 * tau20l_ee);
           tau_trg_mc = Efficiency(t_pt, 14.4451, 0.0790573, 0.0732472, 1.47046, 0.942028);
         }
       } else if (mc_ == mc::spring15_74X){ //fall15 only to exercise code, SF's are applicable to spring15 only
         if(pt<100){
           mu_trg = mt_trig_data_->GetBinContent(mt_trig_data_->GetXaxis()->FindBin(m_eta),mt_trig_data_->GetYaxis()->FindBin(pt));
           mu_trg_mc = mt_trig_mc_->GetBinContent(mt_trig_mc_->GetXaxis()->FindBin(m_eta),mt_trig_mc_->GetYaxis()->FindBin(pt));
         } else {
           mu_trg = mt_trig_data_->GetBinContent(mt_trig_data_->GetXaxis()->FindBin(m_eta),(mt_trig_data_->GetYaxis()->FindBin(pt)-1));
           mu_trg_mc = mt_trig_mc_->GetBinContent(mt_trig_mc_->GetXaxis()->FindBin(m_eta),(mt_trig_mc_->GetYaxis()->FindBin(pt)-1));
         }         
         tau_trg=1;
         tau_trg_mc=1;
       } else if (mc_ == mc::fall15_76X ){
         if(pt<150){
           mu_trg = mt_trig_data_->GetBinContent(mt_trig_data_->GetXaxis()->FindBin(m_eta),mt_trig_data_->GetYaxis()->FindBin(pt));
           mu_trg_mc = mt_trig_mc_->GetBinContent(mt_trig_mc_->GetXaxis()->FindBin(m_eta),mt_trig_mc_->GetYaxis()->FindBin(pt));
         } else {
           mu_trg = mt_trig_data_->GetBinContent(mt_trig_data_->GetXaxis()->FindBin(m_eta),(mt_trig_data_->GetYaxis()->FindBin(pt)-1));
           mu_trg_mc = mt_trig_mc_->GetBinContent(mt_trig_mc_->GetXaxis()->FindBin(m_eta),(mt_trig_mc_->GetYaxis()->FindBin(pt)-1));
         }         
         tau_trg=1;
         tau_trg_mc=1;

        } else if(mc_ == mc::spring16_80X || mc_ == mc::summer16_80X){
         if(scalefactor_file_=="") {
             if(do_single_lepton_trg_ && !do_cross_trg_){
               if(m_iso<0.15){
                 if(pt<1000){
                   mu_trg = mt_trig_data_->GetBinContent(mt_trig_data_->GetXaxis()->FindBin(m_eta),mt_trig_data_->GetYaxis()->FindBin(pt));
                  // mu_trg_mc = mt_trig_mc_->GetBinContent(mt_trig_mc_->GetXaxis()->FindBin(m_eta),mt_trig_mc_->GetYaxis()->FindBin(pt));
                 } else {
                   mu_trg = mt_trig_data_->GetBinContent(mt_trig_data_->GetXaxis()->FindBin(m_eta),(mt_trig_data_->GetYaxis()->FindBin(pt)-1));
                   //mu_trg_mc = mt_trig_mc_->GetBinContent(mt_trig_mc_->GetXaxis()->FindBin(m_eta),(mt_trig_mc_->GetYaxis()->FindBin(pt)-1));
                 }         
                 tau_trg=1;
                 tau_trg_mc=1;
               } else if (m_iso<0.25){
                 if(pt<1000){
                   mu_trg = mt_antiiso1_trig_data_->GetBinContent(mt_antiiso1_trig_data_->GetXaxis()->FindBin(m_eta),mt_antiiso1_trig_data_->GetYaxis()->FindBin(pt));
   //                mu_trg_mc = mt_antiiso1_trig_mc_->GetBinContent(mt_antiiso1_trig_mc_->GetXaxis()->FindBin(m_eta),mt_antiiso1_trig_mc_->GetYaxis()->FindBin(pt));
                 } else {
                   mu_trg = mt_antiiso1_trig_data_->GetBinContent(mt_antiiso1_trig_data_->GetXaxis()->FindBin(m_eta),(mt_antiiso1_trig_data_->GetYaxis()->FindBin(pt)-1));
                   //mu_trg_mc = mt_antiiso1_trig_mc_->GetBinContent(mt_antiiso1_trig_mc_->GetXaxis()->FindBin(m_eta),(mt_antiiso1_trig_mc_->GetYaxis()->FindBin(pt)-1));
                 }         
                 tau_trg=1;
                 tau_trg_mc=1;
               } else { //scale factors only derived for iso<0.5!
                 if(pt<1000){
                   mu_trg = mt_antiiso2_trig_data_->GetBinContent(mt_antiiso2_trig_data_->GetXaxis()->FindBin(m_eta),mt_antiiso2_trig_data_->GetYaxis()->FindBin(pt));
                   //mu_trg_mc = mt_antiiso2_trig_mc_->GetBinContent(mt_antiiso2_trig_mc_->GetXaxis()->FindBin(m_eta),mt_antiiso2_trig_mc_->GetYaxis()->FindBin(pt));
                 } else {
                   mu_trg = mt_antiiso2_trig_data_->GetBinContent(mt_antiiso2_trig_data_->GetXaxis()->FindBin(m_eta),(mt_antiiso2_trig_data_->GetYaxis()->FindBin(pt)-1));
                   //mu_trg_mc = mt_antiiso2_trig_mc_->GetBinContent(mt_antiiso2_trig_mc_->GetXaxis()->FindBin(m_eta),(mt_antiiso2_trig_mc_->GetYaxis()->FindBin(pt)-1));
                 }         
                 tau_trg=1;
                 tau_trg_mc=1;
               }
             } else if(do_cross_trg_ &&!do_single_lepton_trg_){
                if(pt<1000){
                 mu_trg = mt_xtrig_data_->GetBinContent(mt_xtrig_data_->GetXaxis()->FindBin(m_eta),mt_xtrig_data_->GetYaxis()->FindBin(pt));
                 mu_trg_mc = mt_xtrig_mc_->GetBinContent(mt_xtrig_mc_->GetXaxis()->FindBin(m_eta),mt_xtrig_mc_->GetYaxis()->FindBin(pt));
               } else {
                 mu_trg = mt_xtrig_data_->GetBinContent(mt_xtrig_data_->GetXaxis()->FindBin(m_eta),(mt_xtrig_data_->GetYaxis()->FindBin(pt)-1));
                 mu_trg_mc = mt_xtrig_mc_->GetBinContent(mt_xtrig_mc_->GetXaxis()->FindBin(m_eta),(mt_xtrig_mc_->GetYaxis()->FindBin(pt)-1));
               }         
               tau_trg_mc=1;
               unsigned gm2_ = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_2"));
               if(gm2_ == 5){ //Using tight iso:
                 tau_trg       = Efficiency(t_pt, 21.1744, 0.773395, 0.705463, 1.65358, 1.0000);
               } else {
                 tau_trg       = Efficiency(t_pt,  20.7216, 1.86861, 1.79281, 1.56784, 9.86642e-01);
               } 
             } else if (do_single_lepton_trg_ && do_cross_trg_){
               double mu_sgl_trg = 1.0;
               double mu_sgl_trg_mc = 1.0;
               double mu_cond_trg = 1.0;
               double mu_cond_trg_mc = 1.0;
                if(pt<1000){
                 mu_trg = mt_xtrig_data_->GetBinContent(mt_xtrig_data_->GetXaxis()->FindBin(m_eta),mt_xtrig_data_->GetYaxis()->FindBin(pt));
                 mu_trg_mc = mt_xtrig_mc_->GetBinContent(mt_xtrig_mc_->GetXaxis()->FindBin(m_eta),mt_xtrig_mc_->GetYaxis()->FindBin(pt));
                 mu_sgl_trg = mt_trig_data_->GetBinContent(mt_trig_data_->GetXaxis()->FindBin(m_eta),mt_trig_data_->GetYaxis()->FindBin(pt));
                 mu_sgl_trg_mc = mt_trig_mc_->GetBinContent(mt_trig_mc_->GetXaxis()->FindBin(m_eta),mt_trig_mc_->GetYaxis()->FindBin(pt));
                 mu_cond_trg = mt_conditional_data_->GetBinContent(mt_conditional_data_->GetXaxis()->FindBin(m_eta),mt_conditional_data_->GetYaxis()->FindBin(pt));
                 mu_cond_trg_mc = mt_conditional_mc_->GetBinContent(mt_conditional_mc_->GetXaxis()->FindBin(m_eta),mt_conditional_mc_->GetYaxis()->FindBin(pt));
               } else {
                 mu_trg = mt_xtrig_data_->GetBinContent(mt_xtrig_data_->GetXaxis()->FindBin(m_eta),(mt_xtrig_data_->GetYaxis()->FindBin(pt)-1));
                 mu_trg_mc = mt_xtrig_mc_->GetBinContent(mt_xtrig_mc_->GetXaxis()->FindBin(m_eta),(mt_xtrig_mc_->GetYaxis()->FindBin(pt)-1));
                 mu_sgl_trg = mt_trig_data_->GetBinContent(mt_trig_data_->GetXaxis()->FindBin(m_eta),mt_trig_data_->GetYaxis()->FindBin(pt)-1);
                 mu_sgl_trg_mc = mt_trig_mc_->GetBinContent(mt_trig_mc_->GetXaxis()->FindBin(m_eta),mt_trig_mc_->GetYaxis()->FindBin(pt)-1);
                 mu_cond_trg = mt_conditional_data_->GetBinContent(mt_conditional_data_->GetXaxis()->FindBin(m_eta),mt_conditional_data_->GetYaxis()->FindBin(pt)-1);
                 mu_cond_trg_mc = mt_conditional_mc_->GetBinContent(mt_conditional_mc_->GetXaxis()->FindBin(m_eta),mt_conditional_mc_->GetYaxis()->FindBin(pt)-1);
               }         
               tau_trg_mc=1;
               unsigned gm2_ = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_2"));
               if(gm2_ == 5){ //Using tight iso:
                 tau_trg       = Efficiency(t_pt, 21.1744, 0.773395, 0.705463, 1.65358, 1.0000);
               } else {
                 tau_trg       = Efficiency(t_pt,  20.7216, 1.86861, 1.79281, 1.56784, 9.86642e-01);
               } 
               mu_trg = mu_sgl_trg+tau_trg*mu_trg-tau_trg*mu_cond_trg*mu_trg; //Conditional probability: P(single)+P(crosstrig)-P(crosstrig)*P(singlelept|muleg crosstrig)
               mu_trg_mc = mu_sgl_trg_mc+tau_trg_mc*mu_trg_mc-tau_trg_mc*mu_cond_trg_mc*mu_trg_mc; //Conditional probability: P(single)+P(crosstrig)-P(crosstrig)*P(singlelept|muleg crosstrig)
               tau_trg = 1.0; //In this case full weight is stored as mu_trg weight 
             }
           } else {
               if(do_single_lepton_trg_ && !do_cross_trg_){
                   tau_trg = 1;
                   tau_trg_mc=1;
                   auto args_1 = std::vector<double>{pt,m_signed_eta,m_iso};
                   auto args_desy = std::vector<double>{pt,m_signed_eta};
                   if(mc_ == mc::summer16_80X && strategy_ != strategy::smsummer16 && strategy_ != strategy::cpsummer16 &&  strategy_ != strategy::legacy16 && strategy_ != strategy::cpdecays16){
                     mu_trg_mc = fns_["m_trgOR4_binned_mc"]->eval(args_1.data());
                     mu_trg = fns_["m_trgOR4_binned_data"]->eval(args_1.data()); 
                   } else if(mc_ == mc::summer16_80X && (strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16 || strategy_ == strategy::legacy16 || strategy_ == strategy::cpdecays16)){
      
                     double trigweight_up = 1.0;
                     double trigweight_down = 1.0;
                     if(pt>=23){
                       // use signle muon trigger for pt_1>23
                       if(is_embedded_){
                         mu_trg = fns_["m_trg_binned_data"]->eval(args_1.data());    
                         mu_trg_mc = fns_["m_trg_binned_mc"]->eval(args_1.data());
                       } else {
                         mu_trg_mc = fns_["m_trgMu22OR_eta2p1_desy_mc"]->eval(args_desy.data());
                         mu_trg = fns_["m_trgMu22OR_eta2p1_desy_data"]->eval(args_desy.data()); 
                         if(m_iso>0.15){
                           mu_trg_mc = fns_["m_trgMu22OR_eta2p1_aiso0p15to0p3_desy_mc"]->eval(args_desy.data());
                           mu_trg = fns_["m_trgMu22OR_eta2p1_aiso0p15to0p3_desy_data"]->eval(args_desy.data());    
                         }
                       }
                     } else {
                       trigweight_up=1.05;
                       trigweight_down=0.95;
                       auto t_args = std::vector<double>{t_pt,t_eta};
                       // use cross triggers for pt_1<23
                       unsigned gm2_ = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_2"));
                       if(!is_embedded_){
                         mu_trg_mc = fns_["m_trgMu19leg_eta2p1_desy_mc"]->eval(args_desy.data());
                         mu_trg = fns_["m_trgMu19leg_eta2p1_desy_data"]->eval(args_desy.data());
                         if(m_iso>0.15){
                           mu_trg_mc = fns_["m_trgMu19leg_eta2p1_aiso0p15to0p3_desy_mc"]->eval(args_desy.data());
                           mu_trg = fns_["m_trgMu19leg_eta2p1_aiso0p15to0p3_desy_data"]->eval(args_desy.data());    
                         }
                       } else {
                         mu_trg_mc = fns_["m_trg19_binned_ic_embed"]->eval(args_1.data());
                         mu_trg = fns_["m_trg19_binned_ic_data"]->eval(args_1.data());
                       }
                       if(!is_embedded_){
                         if(gm2_==5) tau_trg = fns_["t_genuine_TightIso_mt_ratio"]->eval(t_args.data());
                         else tau_trg = fns_["t_fake_TightIso_mt_ratio"]->eval(t_args.data());
                       } else {
                         if(gm2_==5) tau_trg = fns_["t_genuine_TightIso_mt_data"]->eval(t_args.data())/fns_["t_TightIso_mt_embed"]->eval(t_args.data());
                         else tau_trg = fns_["t_fake_TightIso_mt_data"]->eval(t_args.data())/fns_["t_TightIso_mt_embed"]->eval(t_args.data());;  
                       }
                     }
                     // may want to add different SFs for anti-iso
                     event->Add("trigweight_up", trigweight_up);
                     event->Add("trigweight_down", trigweight_down);
                   }  else{
                     if(m_iso<0.15 || strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16 ||  strategy_ == strategy::legacy16 || strategy_ == strategy::cpdecays16){
                       mu_trg = fns_["m_trgIsoMu24orTkIsoMu24_desy_data"]->eval(args_desy.data());
                     } else  mu_trg = fns_["m_trgOR_binned_data"]->eval(args_1.data());
                     mu_trg = 1;   
                   }
             
               } else {
                   std::cout << "Cross trigger not currently supported! Setting trigger efficiencies to 1" << std::endl;
                   tau_trg=1;
                   tau_trg_mc=1;
                   mu_trg=1;
                   mu_trg_mc=1;
               }
           }
        } else if (mc_ == mc::mc2017 || mc_ == mc::mc2018 || mc_ == mc::mcleg2016) {
            auto args_1 = std::vector<double>{pt,m_signed_eta,m_iso};  
            mu_trg = fns_["m_trg_binned_data"]->eval(args_1.data());
            if(!is_embedded_) mu_trg_mc = fns_["m_trg_binned_mc"]->eval(args_1.data());
            else mu_trg_mc = fns_["m_trg_binned_embed"]->eval(args_1.data());

            double single_m_sf = mu_trg / mu_trg_mc;
            double t_dm = tau->decay_mode();
            auto args_2 = std::vector<double>{pt,m_signed_eta};  
            auto args_3 = std::vector<double>{t_pt,t_signed_eta,t_phi,t_dm};  
            auto args_4 = std::vector<double>{t_pt,t_dm};
            double t_mvadm = tau->GetTauID("MVADM2017v1");
            auto args_mvadm = std::vector<double>{t_pt,t_mvadm};

            double m_high_pt_cut = 25;
            if(mc_ == mc::mcleg2016) m_high_pt_cut = 23;
            //double t_high_pt_cut = 32;
            //if(mc_ == mc::mcleg2016) t_high_pt_cut = 22;

            double mu_xtrg = fns_["m_crosstrg_data"]->eval(args_1.data());
            double mu_xtrg_mc;
            if(is_embedded_) mu_xtrg_mc = fns_["m_crosstrg_embed"]->eval(args_2.data());
            else mu_xtrg_mc = fns_["m_crosstrg_mc"]->eval(args_2.data());

            double tau_trg_ic=1., tau_trg_mvadm=1.;
            
            if(is_embedded_) {
              //tau_trg_mc = fns_["t_trg_27_embed"]->eval(args_3.data());
              tau_trg_mc = 1.;
              tau_trg = fns_["t_trg_27_embed_ratio"]->eval(args_3.data());
              tau_trg_ic = fns_["t_trg_27_ic_embed_ratio"]->eval(args_4.data());
              tau_trg_mvadm = fns_["t_trg_27_ic_mvadm_embed_ratio"]->eval(args_mvadm.data());

            }
            else {
              //tau_trg_mc = fns_["t_trg_27_mc"]->eval(args_4.data());
              tau_trg_mc = 1.;
              tau_trg = fns_["t_trg_27_ratio"]->eval(args_4.data());
              tau_trg_ic = fns_["t_trg_27_ic_ratio"]->eval(args_4.data());
              tau_trg_mvadm = fns_["t_trg_27_ic_mvadm_ratio"]->eval(args_mvadm.data());
            }

            double xtrg_mt_sf = (mu_xtrg_mc*tau_trg_mc) > 0 ? (mu_xtrg*tau_trg)/(mu_xtrg_mc*tau_trg_mc) : 0.0;

            double xtrg_OR_sf = (pt>=m_high_pt_cut) ? single_m_sf : xtrg_mt_sf ; 

            // if trigger is not applied in the MC then set the SF to the efficiency
            if(!trg_applied_in_mc_) {
              xtrg_mt_sf = mu_xtrg*tau_trg;
              xtrg_OR_sf = mu_trg*(1-tau_trg) + mu_xtrg*tau_trg;
              single_m_sf = mu_trg;
            }


            tau_trg_ic = (tau_trg==0) ? tau_trg_ic : tau_trg_ic/tau_trg;
            tau_trg_mvadm = (tau_trg==0) ? tau_trg_mvadm : tau_trg_mvadm/tau_trg;
            tau_trg_ic = (pt>=m_high_pt_cut) ? 1. : tau_trg_ic; 
            tau_trg_mvadm = (pt>=m_high_pt_cut) ? 1. : tau_trg_mvadm; 

            event->Add("wt_tau_trg_ic",tau_trg_ic);
            event->Add("wt_tau_trg_mvadm",tau_trg_mvadm);

            double tau_trg_dm0_up=1.;
            double tau_trg_dm1_up=1.;
            double tau_trg_dm10_up=1.;
            double tau_trg_dm11_up=1.;
            double tau_trg_dm0_down=1.;
            double tau_trg_dm1_down=1.;
            double tau_trg_dm10_down=1.;
            double tau_trg_dm11_down=1.;

            double tau_trg_mvadm0_up=1.;
            double tau_trg_mvadm1_up=1.;
            double tau_trg_mvadm2_up=1.;
            double tau_trg_mvadm10_up=1.;
            double tau_trg_mvadm11_up=1.;
            double tau_trg_mvadm0_down=1.;
            double tau_trg_mvadm1_down=1.;
            double tau_trg_mvadm2_down=1.;
            double tau_trg_mvadm10_down=1.;
            double tau_trg_mvadm11_down=1.;

            if(pt<m_high_pt_cut) { // if this isn't true then we are using the single lepton trigger and therefore the weight = 1
              if(!is_embedded_) {
                if(fns_["t_trg_27_ratio"]->eval(args_4.data())>0){
                  tau_trg_dm0_up=fns_["t_trg_27_ratio_dm0_up"]->eval(args_4.data())/fns_["t_trg_27_ratio"]->eval(args_4.data());
                  tau_trg_dm1_up=fns_["t_trg_27_ratio_dm1_up"]->eval(args_4.data())/fns_["t_trg_27_ratio"]->eval(args_4.data());
                  tau_trg_dm10_up=fns_["t_trg_27_ratio_dm10_up"]->eval(args_4.data())/fns_["t_trg_27_ratio"]->eval(args_4.data());
                  tau_trg_dm11_up=fns_["t_trg_27_ratio_dm11_up"]->eval(args_4.data())/fns_["t_trg_27_ratio"]->eval(args_4.data());
                  tau_trg_dm0_down=fns_["t_trg_27_ratio_dm0_down"]->eval(args_4.data())/fns_["t_trg_27_ratio"]->eval(args_4.data());
                  tau_trg_dm1_down=fns_["t_trg_27_ratio_dm1_down"]->eval(args_4.data())/fns_["t_trg_27_ratio"]->eval(args_4.data());
                  tau_trg_dm10_down=fns_["t_trg_27_ratio_dm10_down"]->eval(args_4.data())/fns_["t_trg_27_ratio"]->eval(args_4.data());
                  tau_trg_dm11_down=fns_["t_trg_27_ratio_dm11_down"]->eval(args_4.data())/fns_["t_trg_27_ratio"]->eval(args_4.data());
                }
                if(fns_["t_trg_27_ic_mvadm_ratio"]->eval(args_mvadm.data())>0){
                  tau_trg_mvadm0_up   =fns_["t_trg_27_ic_mvadm_ratio_mvadm0_up"]->eval(args_mvadm.data())   /fns_["t_trg_27_ic_mvadm_ratio"]->eval(args_mvadm.data());
                  tau_trg_mvadm1_up   =fns_["t_trg_27_ic_mvadm_ratio_mvadm1_up"]->eval(args_mvadm.data())   /fns_["t_trg_27_ic_mvadm_ratio"]->eval(args_mvadm.data());
                  tau_trg_mvadm2_up   =fns_["t_trg_27_ic_mvadm_ratio_mvadm2_up"]->eval(args_mvadm.data())   /fns_["t_trg_27_ic_mvadm_ratio"]->eval(args_mvadm.data());
                  tau_trg_mvadm10_up  =fns_["t_trg_27_ic_mvadm_ratio_mvadm10_up"]->eval(args_mvadm.data())  /fns_["t_trg_27_ic_mvadm_ratio"]->eval(args_mvadm.data());
                  tau_trg_mvadm11_up  =fns_["t_trg_27_ic_mvadm_ratio_mvadm11_up"]->eval(args_mvadm.data())  /fns_["t_trg_27_ic_mvadm_ratio"]->eval(args_mvadm.data());
                  tau_trg_mvadm0_down =fns_["t_trg_27_ic_mvadm_ratio_mvadm0_down"]->eval(args_mvadm.data()) /fns_["t_trg_27_ic_mvadm_ratio"]->eval(args_mvadm.data());
                  tau_trg_mvadm1_down =fns_["t_trg_27_ic_mvadm_ratio_mvadm1_down"]->eval(args_mvadm.data()) /fns_["t_trg_27_ic_mvadm_ratio"]->eval(args_mvadm.data());
                  tau_trg_mvadm2_down =fns_["t_trg_27_ic_mvadm_ratio_mvadm2_down"]->eval(args_mvadm.data()) /fns_["t_trg_27_ic_mvadm_ratio"]->eval(args_mvadm.data());
                  tau_trg_mvadm10_down=fns_["t_trg_27_ic_mvadm_ratio_mvadm10_down"]->eval(args_mvadm.data())/fns_["t_trg_27_ic_mvadm_ratio"]->eval(args_mvadm.data());
                  tau_trg_mvadm11_down=fns_["t_trg_27_ic_mvadm_ratio_mvadm11_down"]->eval(args_mvadm.data())/fns_["t_trg_27_ic_mvadm_ratio"]->eval(args_mvadm.data());
                }

              } else {
                if(fns_["t_trg_27_embed_ratio"]->eval(args_3.data())>0) {
                  tau_trg_dm0_up=fns_["t_trg_27_embed_ratio_dm0_up"]->eval(args_3.data())/fns_["t_trg_27_embed_ratio"]->eval(args_3.data());
                  tau_trg_dm1_up=fns_["t_trg_27_embed_ratio_dm1_up"]->eval(args_3.data())/fns_["t_trg_27_embed_ratio"]->eval(args_3.data());
                  tau_trg_dm10_up=fns_["t_trg_27_embed_ratio_dm10_up"]->eval(args_3.data())/fns_["t_trg_27_embed_ratio"]->eval(args_3.data());
                  tau_trg_dm11_up=fns_["t_trg_27_embed_ratio_dm11_up"]->eval(args_3.data())/fns_["t_trg_27_embed_ratio"]->eval(args_3.data());
                  tau_trg_dm0_down=fns_["t_trg_27_embed_ratio_dm0_down"]->eval(args_3.data())/fns_["t_trg_27_embed_ratio"]->eval(args_3.data());
                  tau_trg_dm1_down=fns_["t_trg_27_embed_ratio_dm1_down"]->eval(args_3.data())/fns_["t_trg_27_embed_ratio"]->eval(args_3.data());
                  tau_trg_dm10_down=fns_["t_trg_27_embed_ratio_dm10_down"]->eval(args_3.data())/fns_["t_trg_27_embed_ratio"]->eval(args_3.data());
                  tau_trg_dm11_down=fns_["t_trg_27_embed_ratio_dm11_down"]->eval(args_3.data())/fns_["t_trg_27_embed_ratio"]->eval(args_3.data());
                }
                if(fns_["t_trg_27_ic_mvadm_embed_ratio"]->eval(args_mvadm.data())>0){
                  tau_trg_mvadm0_up   =fns_["t_trg_27_ic_mvadm_embed_ratio_mvadm0_up"]->eval(args_mvadm.data())   /fns_["t_trg_27_ic_mvadm_embed_ratio"]->eval(args_mvadm.data());
                  tau_trg_mvadm1_up   =fns_["t_trg_27_ic_mvadm_embed_ratio_mvadm1_up"]->eval(args_mvadm.data())   /fns_["t_trg_27_ic_mvadm_embed_ratio"]->eval(args_mvadm.data());
                  tau_trg_mvadm2_up   =fns_["t_trg_27_ic_mvadm_embed_ratio_mvadm2_up"]->eval(args_mvadm.data())   /fns_["t_trg_27_ic_mvadm_embed_ratio"]->eval(args_mvadm.data());
                  tau_trg_mvadm10_up  =fns_["t_trg_27_ic_mvadm_embed_ratio_mvadm10_up"]->eval(args_mvadm.data())  /fns_["t_trg_27_ic_mvadm_embed_ratio"]->eval(args_mvadm.data());
                  tau_trg_mvadm11_up  =fns_["t_trg_27_ic_mvadm_embed_ratio_mvadm11_up"]->eval(args_mvadm.data())  /fns_["t_trg_27_ic_mvadm_embed_ratio"]->eval(args_mvadm.data());
                  tau_trg_mvadm0_down =fns_["t_trg_27_ic_mvadm_embed_ratio_mvadm0_down"]->eval(args_mvadm.data()) /fns_["t_trg_27_ic_mvadm_embed_ratio"]->eval(args_mvadm.data());
                  tau_trg_mvadm1_down =fns_["t_trg_27_ic_mvadm_embed_ratio_mvadm1_down"]->eval(args_mvadm.data()) /fns_["t_trg_27_ic_mvadm_embed_ratio"]->eval(args_mvadm.data());
                  tau_trg_mvadm2_down =fns_["t_trg_27_ic_mvadm_embed_ratio_mvadm2_down"]->eval(args_mvadm.data()) /fns_["t_trg_27_ic_mvadm_embed_ratio"]->eval(args_mvadm.data());
                  tau_trg_mvadm10_down=fns_["t_trg_27_ic_mvadm_embed_ratio_mvadm10_down"]->eval(args_mvadm.data())/fns_["t_trg_27_ic_mvadm_embed_ratio"]->eval(args_mvadm.data());
                  tau_trg_mvadm11_down=fns_["t_trg_27_ic_mvadm_embed_ratio_mvadm11_down"]->eval(args_mvadm.data())/fns_["t_trg_27_ic_mvadm_embed_ratio"]->eval(args_mvadm.data());
                }

              }
            }

            tau_trg_dm0_up = std::isnan(tau_trg_dm0_up) ? 0 : tau_trg_dm0_up;
            tau_trg_dm0_down = std::isnan(tau_trg_dm0_down) ? 0 : tau_trg_dm0_down;
            tau_trg_dm1_up = std::isnan(tau_trg_dm1_up) ? 0 : tau_trg_dm1_up;
            tau_trg_dm1_down = std::isnan(tau_trg_dm1_down) ? 0 : tau_trg_dm1_down;
            tau_trg_dm10_up = std::isnan(tau_trg_dm10_up) ? 0 : tau_trg_dm10_up;
            tau_trg_dm10_down = std::isnan(tau_trg_dm10_down) ? 0 : tau_trg_dm10_down;
            tau_trg_dm11_up = std::isnan(tau_trg_dm11_up) ? 0 : tau_trg_dm11_up;
            tau_trg_dm11_down = std::isnan(tau_trg_dm11_down) ? 0 : tau_trg_dm11_down;

            tau_trg_mvadm0_up    = std::isnan(tau_trg_mvadm0_up)    ? 0 : tau_trg_mvadm0_up;
            tau_trg_mvadm0_down  = std::isnan(tau_trg_mvadm0_down)  ? 0 : tau_trg_mvadm0_down;
            tau_trg_mvadm1_up    = std::isnan(tau_trg_mvadm1_up)    ? 0 : tau_trg_mvadm1_up;
            tau_trg_mvadm1_down  = std::isnan(tau_trg_mvadm1_down)  ? 0 : tau_trg_mvadm1_down;
            tau_trg_mvadm2_up    = std::isnan(tau_trg_mvadm2_up)    ? 0 : tau_trg_mvadm2_up;
            tau_trg_mvadm2_down  = std::isnan(tau_trg_mvadm2_down)  ? 0 : tau_trg_mvadm2_down;
            tau_trg_mvadm10_up   = std::isnan(tau_trg_mvadm10_up)   ? 0 : tau_trg_mvadm10_up;
            tau_trg_mvadm10_down = std::isnan(tau_trg_mvadm10_down) ? 0 : tau_trg_mvadm10_down;
            tau_trg_mvadm11_up   = std::isnan(tau_trg_mvadm11_up)   ? 0 : tau_trg_mvadm11_up;
            tau_trg_mvadm11_down = std::isnan(tau_trg_mvadm11_down) ? 0 : tau_trg_mvadm11_down;

            event->Add("wt_tau_trg_dm0_up",tau_trg_dm0_up);
            event->Add("wt_tau_trg_dm1_up",tau_trg_dm1_up);
            event->Add("wt_tau_trg_dm10_up",tau_trg_dm10_up);
            event->Add("wt_tau_trg_dm11_up",tau_trg_dm11_up);
            event->Add("wt_tau_trg_dm0_down",tau_trg_dm0_down);
            event->Add("wt_tau_trg_dm1_down",tau_trg_dm1_down);
            event->Add("wt_tau_trg_dm10_down",tau_trg_dm10_down);
            event->Add("wt_tau_trg_dm11_down",tau_trg_dm11_down);
 
            event->Add("wt_tau_trg_mvadm0_up"   ,tau_trg_mvadm0_up);
            event->Add("wt_tau_trg_mvadm1_up"   ,tau_trg_mvadm1_up);
            event->Add("wt_tau_trg_mvadm2_up"   ,tau_trg_mvadm2_up);
            event->Add("wt_tau_trg_mvadm10_up"  ,tau_trg_mvadm10_up);
            event->Add("wt_tau_trg_mvadm11_up"  ,tau_trg_mvadm11_up);
            event->Add("wt_tau_trg_mvadm0_down" ,tau_trg_mvadm0_down);
            event->Add("wt_tau_trg_mvadm1_down" ,tau_trg_mvadm1_down);
            event->Add("wt_tau_trg_mvadm2_down" ,tau_trg_mvadm2_down);
            event->Add("wt_tau_trg_mvadm10_down",tau_trg_mvadm10_down);
            event->Add("wt_tau_trg_mvadm11_down",tau_trg_mvadm11_down);

 
            // have xtrg OR as default but save others to check 
            event->Add("single_l_sf", xtrg_OR_sf==0 ? single_m_sf : single_m_sf/xtrg_OR_sf);
            event->Add("xtrg_sf", xtrg_OR_sf==0 ? xtrg_mt_sf : xtrg_mt_sf/xtrg_OR_sf);

            mu_trg = xtrg_OR_sf;
            mu_trg_mc = 1.0;

            tau_trg = 1.0;
            tau_trg_mc = 1.0; 
       }

       if (trg_applied_in_mc_) {
         mu_trg = mu_trg / mu_trg_mc;
         tau_trg = tau_trg / tau_trg_mc;
       }
       if(mu_trg>2) mu_trg=2;
       weight *= (mu_trg * tau_trg);
       event->Add("trigweight_1", mu_trg);
       event->Add("trigweight_2", tau_trg);
     } else if (channel_ == channel::em) {
       Electron const* elec = dynamic_cast<Electron const*>(dilepton[0]->GetCandidate("lepton1"));
       Muon const* muon = dynamic_cast<Muon const*>(dilepton[0]->GetCandidate("lepton2"));
       double e_pt = elec->pt();
       double e_eta = fabs(elec->eta());
       double e_iso = PF03IsolationVal(elec, 0.5, 0);
       double m_pt = muon->pt();
       double m_eta = fabs(muon->eta());
       double m_iso = PF04IsolationVal(muon, 0.5, 0);
       double m_trg = 1.0;
       double m_trg_mc = 1.0;
       double e_trg = 1.0;
       double e_trg_mc = 1.0;
       double m_trg_17 = 1.0;
       double m_trg_17_mc = 1.0;
       double m_trg_23 = 1.0;
       double m_trg_23_mc = 1.0;
       double m_trg_8 = 1.0;
       double m_trg_8_mc = 1.0;
       double e_trg_17 = 1.0;
       double e_trg_17_mc = 1.0;
       double e_trg_23 = 1.0;
       double e_trg_23_mc = 1.0;
       double e_trg_12 = 1.0;
       double e_trg_12_mc = 1.0;
       if (mc_ == mc::summer12_53X) {
         if (era_ == era::data_2012_rereco) {
           if (m_eta < 0.8) {
             if (m_pt <= 15.0)                { m_trg_mc = 0.9870; m_trg = 0.9701; }
             if (m_pt > 15.0 && m_pt <= 20.0) { m_trg_mc = 0.9868; m_trg = 0.9720; }
             if (m_pt > 20.0 && m_pt <= 25.0) { m_trg_mc = 0.9812; m_trg = 0.9764; }
             if (m_pt > 25.0 && m_pt <= 30.0) { m_trg_mc = 0.9853; m_trg = 0.9725; }
             if (m_pt > 30.0 && m_pt <= 35.0) { m_trg_mc = 0.9826; m_trg = 0.9785; }
             if (m_pt > 35.0)                 { m_trg_mc = 0.9692; m_trg = 0.9679; }
           } else if (m_eta >= 0.8 && m_eta < 1.2) {
             if (m_pt <= 15.0)                { m_trg_mc = 0.9666; m_trg = 0.9419; }
             if (m_pt > 15.0 && m_pt <= 20.0) { m_trg_mc = 0.9445; m_trg = 0.9305; }
             if (m_pt > 20.0 && m_pt <= 25.0) { m_trg_mc = 0.9822; m_trg = 0.9439; }
             if (m_pt > 25.0 && m_pt <= 30.0) { m_trg_mc = 0.9618; m_trg = 0.9405; }
             if (m_pt > 30.0 && m_pt <= 35.0) { m_trg_mc = 0.9454; m_trg = 0.9342; }
             if (m_pt > 35.0)                 { m_trg_mc = 0.9759; m_trg = 0.9310; }
           } else if (m_eta >= 1.2 && m_eta < 1.6) {
             if (m_pt <= 15.0)                { m_trg_mc = 0.9357; m_trg = 0.9303; }
             if (m_pt > 15.0 && m_pt <= 20.0) { m_trg_mc = 0.9512; m_trg = 0.9267; }
             if (m_pt > 20.0 && m_pt <= 25.0) { m_trg_mc = 0.9640; m_trg = 0.9366; }
             if (m_pt > 25.0 && m_pt <= 30.0) { m_trg_mc = 0.9538; m_trg = 0.9218; }
             if (m_pt > 30.0 && m_pt <= 35.0) { m_trg_mc = 0.9247; m_trg = 0.9184; }
             if (m_pt > 35.0)                 { m_trg_mc = 0.9522; m_trg = 0.9092; }
           } else {
             if (m_pt <= 15.0)                { m_trg_mc = 0.9415; m_trg = 0.8623; }
             if (m_pt > 15.0 && m_pt <= 20.0) { m_trg_mc = 0.9638; m_trg = 0.8995; }
             if (m_pt > 20.0 && m_pt <= 25.0) { m_trg_mc = 0.9657; m_trg = 0.9134; }
             if (m_pt > 25.0 && m_pt <= 30.0) { m_trg_mc = 0.9287; m_trg = 0.8824; }
             if (m_pt > 30.0 && m_pt <= 35.0) { m_trg_mc = 0.9573; m_trg = 0.8990; }
             if (m_pt > 35.0)                 { m_trg_mc = 0.9606; m_trg = 0.9016; }
           }
           if (e_eta < 0.8) {
             if (e_pt <= 15.0)                { e_trg_mc = 0.7615; e_trg = 0.7270; }
             if (e_pt > 15.0 && e_pt <= 20.0) { e_trg_mc = 0.8903; e_trg = 0.8752; }
             if (e_pt > 20.0 && e_pt <= 25.0) { e_trg_mc = 0.9419; e_trg = 0.9142; }
             if (e_pt > 25.0 && e_pt <= 30.0) { e_trg_mc = 0.9591; e_trg = 0.9368; }
             if (e_pt > 30.0 && e_pt <= 35.0) { e_trg_mc = 0.9454; e_trg = 0.9499; }
             if (e_pt > 35.0)                 { e_trg_mc = 0.9629; e_trg = 0.9689; }
           } else if (e_eta >= 0.8 && e_eta < 1.479) {
             if (e_pt <= 15.0)                { e_trg_mc = 0.8186; e_trg = 0.7380; }
             if (e_pt > 15.0 && e_pt <= 20.0) { e_trg_mc = 0.9367; e_trg = 0.9059; }
             if (e_pt > 20.0 && e_pt <= 25.0) { e_trg_mc = 0.9746; e_trg = 0.9484; }
             if (e_pt > 25.0 && e_pt <= 30.0) { e_trg_mc = 0.9757; e_trg = 0.9630; }
             if (e_pt > 30.0 && e_pt <= 35.0) { e_trg_mc = 0.9748; e_trg = 0.9642; }
             if (e_pt > 35.0)                 { e_trg_mc = 0.9763; e_trg = 0.9809; }
           } else {
             if (e_pt <= 15.0)                { e_trg_mc = 0.7652; e_trg = 0.6899; }
             if (e_pt > 15.0 && e_pt <= 20.0) { e_trg_mc = 0.9126; e_trg = 0.8635; }
             if (e_pt > 20.0 && e_pt <= 25.0) { e_trg_mc = 0.9654; e_trg = 0.9356; }
             if (e_pt > 25.0 && e_pt <= 30.0) { e_trg_mc = 0.9732; e_trg = 0.9466; }
             if (e_pt > 30.0 && e_pt <= 35.0) { e_trg_mc = 0.9875; e_trg = 0.9735; }
             if (e_pt > 35.0)                 { e_trg_mc = 0.9787; e_trg = 0.9802; }
           }
         }
       } else if (mc_ == mc::fall11_42X) {
         if (m_eta < 0.8) {
           if (m_pt <= 15.0)                 { m_trg_mc = 0.984910277; m_trg = 0.9660; }
           if (m_pt > 15.0 && m_pt <= 20.0)  { m_trg_mc = 0.985725938; m_trg = 0.9668; }
           if (m_pt > 20.0 && m_pt <= 25.0)  { m_trg_mc = 0.984354758; m_trg = 0.9878; }
           if (m_pt > 25.0 && m_pt <= 30.0)  { m_trg_mc = 0.986547085; m_trg = 0.9680; }
           if (m_pt > 30.0)                  { m_trg_mc = 0.988551569; m_trg = 0.9671; }
         } else if (m_eta >= 0.8 && m_eta < 1.2) {
           if (m_pt <= 15.0)                 { m_trg_mc = 0.967889432; m_trg = 0.9314; }
           if (m_pt > 15.0 && m_pt <= 20.0)  { m_trg_mc = 0.975998366; m_trg = 0.9556; }
           if (m_pt > 20.0 && m_pt <= 25.0)  { m_trg_mc = 0.979876161; m_trg = 0.9495; }
           if (m_pt > 25.0 && m_pt <= 30.0)  { m_trg_mc = 0.976183175; m_trg = 0.9550; }
           if (m_pt > 30.0)                  { m_trg_mc = 0.982728307; m_trg = 0.9502; }
         } else {
           if (m_pt <= 15.0)                 { m_trg_mc = 0.958862737; m_trg = 0.9207; }
           if (m_pt > 15.0 && m_pt <= 20.0)  { m_trg_mc = 0.966324889; m_trg = 0.9613; }
           if (m_pt > 20.0 && m_pt <= 25.0)  { m_trg_mc = 0.967505674; m_trg = 0.9379; }
           if (m_pt > 25.0 && m_pt <= 30.0)  { m_trg_mc = 0.970721649; m_trg = 0.9416; }
           if (m_pt > 30.0)                  { m_trg_mc = 0.969919372; m_trg = 0.9383; }
         }
         if (e_eta < 0.8) {
           if (e_pt <= 15.0)                 { e_trg_mc = 0.786907216; e_trg = 0.7633; }
           if (e_pt > 15.0 && e_pt <= 20.0)  { e_trg_mc = 0.806727273; e_trg = 0.8874; }
           if (e_pt > 20.0 && e_pt <= 25.0)  { e_trg_mc = 0.938775510; e_trg = 0.9200; }
           if (e_pt > 25.0 && e_pt <= 30.0)  { e_trg_mc = 0.958571429; e_trg = 0.9394; }
           if (e_pt > 30.0)                  { e_trg_mc = 0.974040404; e_trg = 0.9643; }
         } else if (e_eta >= 0.8 && e_eta < 1.479) {
           if (e_pt <= 15.0)                 { e_trg_mc = 0.750612245; e_trg = 0.7356; }
           if (e_pt > 15.0 && e_pt <= 20.0)  { e_trg_mc = 0.849722222; e_trg = 0.9177; }
           if (e_pt > 20.0 && e_pt <= 25.0)  { e_trg_mc = 0.961111111; e_trg = 0.9515; }
           if (e_pt > 25.0 && e_pt <= 30.0)  { e_trg_mc = 0.977171717; e_trg = 0.9674; }
           if (e_pt > 30.0)                  { e_trg_mc = 0.977800000; e_trg = 0.9778; }
         } else {
           if (e_pt <= 15.0)                 { e_trg_mc = 0.865432099; e_trg = 0.7010; }
           if (e_pt > 15.0 && e_pt <= 20.0)  { e_trg_mc = 0.809523810; e_trg = 0.8500; }
           if (e_pt > 20.0 && e_pt <= 25.0)  { e_trg_mc = 0.971145833; e_trg = 0.9323; }
           if (e_pt > 25.0 && e_pt <= 30.0)  { e_trg_mc = 0.967291667; e_trg = 0.9286; }
           if (e_pt > 30.0)                  { e_trg_mc = 0.983535354; e_trg = 0.9737; }
         }
       } else if (mc_ == mc::spring15_74X){
         if(e_pt<100){
           e_trg_17 = em_e17_trig_data_->GetBinContent(em_e17_trig_data_->GetXaxis()->FindBin(e_eta),em_e17_trig_data_->GetYaxis()->FindBin(e_pt));
           e_trg_17_mc = em_e17_trig_mc_->GetBinContent(em_e17_trig_mc_->GetXaxis()->FindBin(e_eta),em_e17_trig_mc_->GetYaxis()->FindBin(e_pt));
           e_trg_12 = em_e12_trig_data_->GetBinContent(em_e12_trig_data_->GetXaxis()->FindBin(e_eta),em_e12_trig_data_->GetYaxis()->FindBin(e_pt));
           e_trg_12_mc = em_e12_trig_mc_->GetBinContent(em_e12_trig_mc_->GetXaxis()->FindBin(e_eta),em_e12_trig_mc_->GetYaxis()->FindBin(e_pt));
         } else {
           e_trg_17 = em_e17_trig_data_->GetBinContent(em_e17_trig_data_->GetXaxis()->FindBin(e_eta),(em_e17_trig_data_->GetYaxis()->FindBin(e_pt)-1));
           e_trg_17_mc = em_e17_trig_mc_->GetBinContent(em_e17_trig_mc_->GetXaxis()->FindBin(e_eta),(em_e17_trig_mc_->GetYaxis()->FindBin(e_pt)-1));
           e_trg_12 = em_e12_trig_data_->GetBinContent(em_e12_trig_data_->GetXaxis()->FindBin(e_eta),(em_e12_trig_data_->GetYaxis()->FindBin(e_pt)-1));
           e_trg_12_mc = em_e12_trig_mc_->GetBinContent(em_e12_trig_mc_->GetXaxis()->FindBin(e_eta),(em_e12_trig_mc_->GetYaxis()->FindBin(e_pt)-1));
         }         
         if(m_pt<100){
           m_trg_17 = em_m17_trig_data_->GetBinContent(em_m17_trig_data_->GetXaxis()->FindBin(m_eta),em_m17_trig_data_->GetYaxis()->FindBin(m_pt));
           m_trg_17_mc = em_m17_trig_mc_->GetBinContent(em_m17_trig_mc_->GetXaxis()->FindBin(m_eta),em_m17_trig_mc_->GetYaxis()->FindBin(m_pt));
           m_trg_8 = em_m8_trig_data_->GetBinContent(em_m8_trig_data_->GetXaxis()->FindBin(m_eta),em_m8_trig_data_->GetYaxis()->FindBin(m_pt));
           m_trg_8_mc = em_m8_trig_mc_->GetBinContent(em_m8_trig_mc_->GetXaxis()->FindBin(m_eta),em_m8_trig_mc_->GetYaxis()->FindBin(m_pt));
         } else {
           m_trg_17 = em_m17_trig_data_->GetBinContent(em_m17_trig_data_->GetXaxis()->FindBin(m_eta),(em_m17_trig_data_->GetYaxis()->FindBin(m_pt)-1));
           m_trg_17_mc = em_m17_trig_mc_->GetBinContent(em_m17_trig_mc_->GetXaxis()->FindBin(m_eta),(em_m17_trig_mc_->GetYaxis()->FindBin(m_pt)-1));
           m_trg_8 = em_m8_trig_data_->GetBinContent(em_m8_trig_data_->GetXaxis()->FindBin(m_eta),(em_m8_trig_data_->GetYaxis()->FindBin(m_pt)-1));
           m_trg_8_mc = em_m8_trig_mc_->GetBinContent(em_m8_trig_mc_->GetXaxis()->FindBin(m_eta),(em_m8_trig_mc_->GetYaxis()->FindBin(m_pt)-1));
         }         
      } else if (mc_ == mc::fall15_76X){
         if(e_pt<150){
           e_trg_17 = em_e17_trig_data_->GetBinContent(em_e17_trig_data_->GetXaxis()->FindBin(e_eta),em_e17_trig_data_->GetYaxis()->FindBin(e_pt));
           e_trg_17_mc = em_e17_trig_mc_->GetBinContent(em_e17_trig_mc_->GetXaxis()->FindBin(e_eta),em_e17_trig_mc_->GetYaxis()->FindBin(e_pt));
           e_trg_12 = em_e12_trig_data_->GetBinContent(em_e12_trig_data_->GetXaxis()->FindBin(e_eta),em_e12_trig_data_->GetYaxis()->FindBin(e_pt));
           e_trg_12_mc = em_e12_trig_mc_->GetBinContent(em_e12_trig_mc_->GetXaxis()->FindBin(e_eta),em_e12_trig_mc_->GetYaxis()->FindBin(e_pt));
         } else {
           e_trg_17 = em_e17_trig_data_->GetBinContent(em_e17_trig_data_->GetXaxis()->FindBin(e_eta),(em_e17_trig_data_->GetYaxis()->FindBin(e_pt)-1));
           e_trg_17_mc = em_e17_trig_mc_->GetBinContent(em_e17_trig_mc_->GetXaxis()->FindBin(e_eta),(em_e17_trig_mc_->GetYaxis()->FindBin(e_pt)-1));
           e_trg_12 = em_e12_trig_data_->GetBinContent(em_e12_trig_data_->GetXaxis()->FindBin(e_eta),(em_e12_trig_data_->GetYaxis()->FindBin(e_pt)-1));
           e_trg_12_mc = em_e12_trig_mc_->GetBinContent(em_e12_trig_mc_->GetXaxis()->FindBin(e_eta),(em_e12_trig_mc_->GetYaxis()->FindBin(e_pt)-1));
         }         
         if(m_pt<150){
           m_trg_17 = em_m17_trig_data_->GetBinContent(em_m17_trig_data_->GetXaxis()->FindBin(m_eta),em_m17_trig_data_->GetYaxis()->FindBin(m_pt));
           m_trg_17_mc = em_m17_trig_mc_->GetBinContent(em_m17_trig_mc_->GetXaxis()->FindBin(m_eta),em_m17_trig_mc_->GetYaxis()->FindBin(m_pt));
           m_trg_8 = em_m8_trig_data_->GetBinContent(em_m8_trig_data_->GetXaxis()->FindBin(m_eta),em_m8_trig_data_->GetYaxis()->FindBin(m_pt));
           m_trg_8_mc = em_m8_trig_mc_->GetBinContent(em_m8_trig_mc_->GetXaxis()->FindBin(m_eta),em_m8_trig_mc_->GetYaxis()->FindBin(m_pt));
         } else {
           m_trg_17 = em_m17_trig_data_->GetBinContent(em_m17_trig_data_->GetXaxis()->FindBin(m_eta),(em_m17_trig_data_->GetYaxis()->FindBin(m_pt)-1));
           m_trg_17_mc = em_m17_trig_mc_->GetBinContent(em_m17_trig_mc_->GetXaxis()->FindBin(m_eta),(em_m17_trig_mc_->GetYaxis()->FindBin(m_pt)-1));
           m_trg_8 = em_m8_trig_data_->GetBinContent(em_m8_trig_data_->GetXaxis()->FindBin(m_eta),(em_m8_trig_data_->GetYaxis()->FindBin(m_pt)-1));
           m_trg_8_mc = em_m8_trig_mc_->GetBinContent(em_m8_trig_mc_->GetXaxis()->FindBin(m_eta),(em_m8_trig_mc_->GetYaxis()->FindBin(m_pt)-1));
         }         

      } else if (mc_ == mc::spring16_80X || mc_ == mc::summer16_80X){
     /*    if(e_pt<1000){

           e_trg_17 = em_e17_trig_data_->GetBinContent(em_e17_trig_data_->GetXaxis()->FindBin(e_eta),em_e17_trig_data_->GetYaxis()->FindBin(e_pt));
           e_trg_17_mc = em_e17_trig_mc_->GetBinContent(em_e17_trig_mc_->GetXaxis()->FindBin(e_eta),em_e17_trig_mc_->GetYaxis()->FindBin(e_pt));
           e_trg_12 = em_e12_trig_data_->GetBinContent(em_e12_trig_data_->GetXaxis()->FindBin(e_eta),em_e12_trig_data_->GetYaxis()->FindBin(e_pt));
           e_trg_12_mc = em_e12_trig_mc_->GetBinContent(em_e12_trig_mc_->GetXaxis()->FindBin(e_eta),em_e12_trig_mc_->GetYaxis()->FindBin(e_pt));
         } else {
           e_trg_17 = em_e17_trig_data_->GetBinContent(em_e17_trig_data_->GetXaxis()->FindBin(e_eta),(em_e17_trig_data_->GetYaxis()->FindBin(e_pt)-1));
           e_trg_17_mc = em_e17_trig_mc_->GetBinContent(em_e17_trig_mc_->GetXaxis()->FindBin(e_eta),(em_e17_trig_mc_->GetYaxis()->FindBin(e_pt)-1));
           e_trg_12 = em_e12_trig_data_->GetBinContent(em_e12_trig_data_->GetXaxis()->FindBin(e_eta),(em_e12_trig_data_->GetYaxis()->FindBin(e_pt)-1));
           e_trg_12_mc = em_e12_trig_mc_->GetBinContent(em_e12_trig_mc_->GetXaxis()->FindBin(e_eta),(em_e12_trig_mc_->GetYaxis()->FindBin(e_pt)-1));
         }         
         if(m_pt<1000){
           m_trg_17 = em_m17_trig_data_->GetBinContent(em_m17_trig_data_->GetXaxis()->FindBin(m_eta),em_m17_trig_data_->GetYaxis()->FindBin(m_pt));
           m_trg_17_mc = em_m17_trig_mc_->GetBinContent(em_m17_trig_mc_->GetXaxis()->FindBin(m_eta),em_m17_trig_mc_->GetYaxis()->FindBin(m_pt));
           m_trg_8 = em_m8_trig_data_->GetBinContent(em_m8_trig_data_->GetXaxis()->FindBin(m_eta),em_m8_trig_data_->GetYaxis()->FindBin(m_pt));
           m_trg_8_mc = em_m8_trig_mc_->GetBinContent(em_m8_trig_mc_->GetXaxis()->FindBin(m_eta),em_m8_trig_mc_->GetYaxis()->FindBin(m_pt));
         } else {
           m_trg_17 = em_m17_trig_data_->GetBinContent(em_m17_trig_data_->GetXaxis()->FindBin(m_eta),(em_m17_trig_data_->GetYaxis()->FindBin(m_pt)-1));
           m_trg_17_mc = em_m17_trig_mc_->GetBinContent(em_m17_trig_mc_->GetXaxis()->FindBin(m_eta),(em_m17_trig_mc_->GetYaxis()->FindBin(m_pt)-1));
           m_trg_8 = em_m8_trig_data_->GetBinContent(em_m8_trig_data_->GetXaxis()->FindBin(m_eta),(em_m8_trig_data_->GetYaxis()->FindBin(m_pt)-1));
           m_trg_8_mc = em_m8_trig_mc_->GetBinContent(em_m8_trig_mc_->GetXaxis()->FindBin(m_eta),(em_m8_trig_mc_->GetYaxis()->FindBin(m_pt)-1));
         }*/         
         auto args_1 = std::vector<double>{e_pt,e_eta};
         auto args_2 = std::vector<double>{m_pt,m_eta};
         auto args_1_2 = std::vector<double>{e_pt,e_eta,e_iso};
         auto args_2_2 = std::vector<double>{m_pt,m_eta,m_iso};
         //17 is actually 23 but just so I don't have to change the bit below where the efficiency is calculated. Probably one to fix at some point
         if(!is_embedded_){
           m_trg_17 = fns_["m_trgMu23leg_desy_data"]->eval(args_2.data());
           m_trg_8  = fns_["m_trgMu8leg_desy_data"]->eval(args_2.data());
           e_trg_17 = fns_["e_trgEle23leg_desy_data"]->eval(args_1.data());
           e_trg_12  = fns_["e_trgEle12leg_desy_data"]->eval(args_1.data());
           
           if(mc_ == mc::summer16_80X){
             m_trg_17_mc = fns_["m_trgMu23leg_desy_mc"]->eval(args_2.data());
             m_trg_8_mc  = fns_["m_trgMu8leg_desy_mc"]->eval(args_2.data());
             e_trg_17_mc = fns_["e_trgEle23leg_desy_mc"]->eval(args_1.data());
             e_trg_12_mc  = fns_["e_trgEle12leg_desy_mc"]->eval(args_1.data());
           }
         } else {
           m_trg_17 = fns_["m_trg23_binned_ic_data"]->eval(args_2_2.data());
           m_trg_8  = fns_["m_trg8_binned_ic_data"]->eval(args_2_2.data());
           e_trg_17 = fns_["e_trg23_binned_ic_data"]->eval(args_1_2.data());
           e_trg_12  = fns_["e_trg12_binned_ic_data"]->eval(args_1_2.data());

           m_trg_17_mc = fns_["m_trg23_binned_ic_embed"]->eval(args_2_2.data());
           m_trg_8_mc  = fns_["m_trg8_binned_ic_embed"]->eval(args_2_2.data());
           e_trg_17_mc = fns_["e_trg23_binned_ic_embed"]->eval(args_1_2.data());
           e_trg_12_mc  = fns_["e_trg12_binned_ic_embed"]->eval(args_1_2.data());
         }
      } else if (mc_==mc::mc2017 || mc_ == mc::mc2018 || mc_ == mc::mcleg2016){ 
          double e_iso = PF03EAIsolationVal(elec, eventInfo->jet_rho()); //lepton_rho

          auto args_1_2 = std::vector<double>{e_pt,e_eta,e_iso};
          auto args_2_2 = std::vector<double>{m_pt,m_eta,m_iso};

          m_trg_23 = fns_["m_trg_binned_23_data"]->eval(args_2_2.data());
          m_trg_8  = fns_["m_trg_binned_8_data"]->eval(args_2_2.data());
          e_trg_23 = fns_["e_trg_binned_23_data"]->eval(args_1_2.data());
          e_trg_12 = fns_["e_trg_binned_12_data"]->eval(args_1_2.data());

          if(is_embedded_){
            m_trg_23_mc = fns_["m_trg_binned_23_embed"]->eval(args_2_2.data());
            m_trg_8_mc  = fns_["m_trg_binned_8_embed"]->eval(args_2_2.data());
            e_trg_23_mc = fns_["e_trg_binned_23_embed"]->eval(args_1_2.data());
            e_trg_12_mc = fns_["e_trg_binned_12_embed"]->eval(args_1_2.data());
          } else {
            m_trg_23_mc = fns_["m_trg_binned_23_mc"]->eval(args_2_2.data());
            m_trg_8_mc  = fns_["m_trg_binned_8_mc"]->eval(args_2_2.data());
            e_trg_23_mc = fns_["e_trg_binned_23_mc"]->eval(args_1_2.data());
            e_trg_12_mc = fns_["e_trg_binned_12_mc"]->eval(args_1_2.data());
          }
      }
      if(mc_ !=mc::spring15_74X && mc_ != mc::fall15_76X && mc_!=mc::spring16_80X && mc_ != mc::summer16_80X && mc_ != mc::mc2017 && mc_ != mc::mc2018 && mc_ != mc::mcleg2016){
       if (trg_applied_in_mc_) {
         m_trg = m_trg / m_trg_mc;
         e_trg = e_trg / e_trg_mc;
       }
       weight *= (e_trg * m_trg);
       event->Add("trigweight_1", e_trg);
       event->Add("trigweight_2", m_trg);
      } else if(mc_ ==mc::spring15_74X || mc_ == mc::fall15_76X || mc_==mc::spring16_80X || mc_ == mc::summer16_80X) {
       if (trg_applied_in_mc_){
         e_trg = (m_trg_17_mc*e_trg_12_mc + m_trg_8_mc*e_trg_17_mc - m_trg_17_mc*e_trg_17_mc) > 0. ? (m_trg_17*e_trg_12 + m_trg_8*e_trg_17 - m_trg_17*e_trg_17)/(m_trg_17_mc*e_trg_12_mc + m_trg_8_mc*e_trg_17_mc - m_trg_17_mc*e_trg_17_mc) : 0.;
       } else e_trg = (m_trg_17*e_trg_12 + m_trg_8*e_trg_17 - m_trg_17*e_trg_17);
       if(e_trg>2.) e_trg=2.;
       double e_trg_after = e_trg;
       if (e_pt <24.) e_trg_after = m_trg_17_mc*e_trg_12_mc > 0. ? (m_trg_17*e_trg_12)/(m_trg_17_mc*e_trg_12_mc) :  0.;
       if (m_pt <24.) e_trg_after = m_trg_8_mc*e_trg_17_mc > 0. ? (m_trg_8*e_trg_17)/(m_trg_8_mc*e_trg_17_mc) : 0.;
       e_trg = e_trg_after;

       weight *= (e_trg);
       //trigweight_1 is actually the full trigger weight because of the way the efficiencies are combined
       event->Add("trigweight_1", e_trg);
       event->Add("trigweight_2", double(1.0));
       if(mc_==mc::summer16_80X) eventInfo->set_weight("filter_eff",double(0.979));
      } else if (mc_ == mc::mc2017 || mc_ == mc::mc2018 || mc_ == mc::mcleg2016) {
       if (trg_applied_in_mc_){
         e_trg = (m_trg_23_mc*e_trg_12_mc + m_trg_8_mc*e_trg_23_mc - m_trg_23_mc*e_trg_23_mc) > 0. ? (m_trg_23*e_trg_12 + m_trg_8*e_trg_23 - m_trg_23*e_trg_23)/(m_trg_23_mc*e_trg_12_mc + m_trg_8_mc*e_trg_23_mc - m_trg_23_mc*e_trg_23_mc) : 0.;
         // if one of the leptons has pT < 24 then we aren't using the OR of the 2 triggrs anymore for the SF logic is different
         if(e_pt<24.) e_trg = m_trg_23_mc*e_trg_12_mc > 0. ? (m_trg_23*e_trg_12)/(m_trg_23_mc*e_trg_12_mc) :  0.;
         if(m_pt<24.) e_trg = m_trg_8_mc*e_trg_23_mc > 0. ? (m_trg_8*e_trg_23)/(m_trg_8_mc*e_trg_23_mc) : 0.;
       } else {
         e_trg = (m_trg_23*e_trg_12 + m_trg_8*e_trg_23 - m_trg_23*e_trg_23);
         if(e_pt<24.) e_trg = m_trg_23*e_trg_12;
         if(m_pt<24.) e_trg = m_trg_8*e_trg_23;
       }

       // these lines may be more correct due to the additonal offline pT cut of 24 GeV on the high leg
       //double e_trg_after = e_trg;
       //if (e_pt <24.) e_trg_after = m_trg_23_mc*e_trg_12_mc > 0. ? (m_trg_23*e_trg_12)/(m_trg_23_mc*e_trg_12_mc) : 0.;
       //if (m_pt <24.) e_trg_after = m_trg_8_mc*e_trg_23_mc > 0. ? (m_trg_8*e_trg_23)/(m_trg_8_mc*e_trg_23_mc) : 0.;
       //e_trg = e_trg_after;

       if(mc_ == mc::mc2017) e_trg*=0.991;
       if(mc_ == mc::mcleg2016)  eventInfo->set_weight("dz_filter_eff",double(0.979));
       if(e_trg>2.) e_trg=2.;
       weight *= (e_trg);
       //trigweight_1 is actually the full trigger weight because of the way the efficiencies are combined
       event->Add("trigweight_1", e_trg);
       event->Add("trigweight_2", double(1.0));

      }
     } else if (channel_ == channel::tt){
       Tau const* tau1 = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton1"));
       Tau const* tau2 = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton2"));
       double pt_1 = tau1->pt();
       double pt_2 = tau2->pt();
       auto args_1 = std::vector<double>{pt_1};
       auto args_2 = std::vector<double>{pt_2};
       double tau1_trg = 1.0;
       double tau1_trg_mc = 1.0;
       double tau1_trg_up = 1.0;
       //double tau1_trg_mc_up = 1.0;
       double tau1_trg_down = 1.0;
       //double tau1_trg_mc_down = 1.0;
       double tau2_trg = 1.0;
       double tau2_trg_mc = 1.0;
       double tau2_trg_up = 1.0;
       //double tau2_trg_mc_up = 1.0;
       double tau2_trg_down = 1.0;
       //double tau2_trg_mc_down = 1.0;
       if (mc_ == mc::fall15_76X){
         tau1_trg      = Efficiency(pt_1, 34.5412, 5.63353, 2.49242, 3.35896, 1.0);
         tau1_trg_mc   = Efficiency(pt_1, 36.0274, 5.89434, 5.82870, 1.83737, 9.58000e-01);
         tau2_trg      = Efficiency(pt_2, 34.5412, 5.63353, 2.49242, 3.35896, 1.0);
         tau2_trg_mc   = Efficiency(pt_2, 36.0274, 5.89434, 5.82870, 1.83737, 9.58000e-01);
         tau1_trg_up        = Efficiency(pt_1, 33.1713, 5.66551, 1.87175, 8.07790, 1.0);
         //tau1_trg_mc_up     = Efficiency(pt_1, 35.6012, 5.98209, 6.09604, 1.68740, 9.87653e-01);
         tau2_trg_up        = Efficiency(pt_2, 33.1713, 5.66551, 1.87175, 8.07790, 1.0);
         //tau2_trg_mc_up     = Efficiency(pt_2, 35.6012, 5.98209, 6.09604, 1.68740, 9.87653e-01);
         tau1_trg_down      = Efficiency(pt_1, 35.6264, 5.30711, 2.81591, 2.40649, 9.99958e-01);
         //tau1_trg_mc_down   = Efficiency(pt_1, 36.2436, 5.58461, 5.12924, 2.05921, 9.32305e-01);
         tau2_trg_down      = Efficiency(pt_2, 35.6264, 5.30711, 2.81591, 2.40649, 9.99958e-01);
         //tau2_trg_mc_down   = Efficiency(pt_2, 36.2436, 5.58461, 5.12924, 2.05921, 9.32305e-01);
        } else if(mc_ == mc::spring16_80X || mc_ == mc::summer16_80X){
         unsigned gm1_ = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_1"));
         unsigned gm2_ = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_2"));
         if(tt_trg_iso_mode_==4) {
             //Hack the binned efficiencies in here
             bool vloose_2 = tau2->GetTauID("byVLooseIsolationMVArun2v1DBoldDMwLT");
             bool loose_2 =  tau2->GetTauID("byLooseIsolationMVArun2v1DBoldDMwLT") ;
             bool medium_2 = tau2->GetTauID("byMediumIsolationMVArun2v1DBoldDMwLT");
             bool tight_2 =  tau2->GetTauID("byTightIsolationMVArun2v1DBoldDMwLT");
             bool vtight_2 =  tau2->GetTauID("byVTightIsolationMVArun2v1DBoldDMwLT");
             bool vvtight_2 =  tau2->GetTauID("byVVTightIsolationMVArun2v1DBoldDMwLT");
             bool vloose_1 = tau1->GetTauID("byVLooseIsolationMVArun2v1DBoldDMwLT");
             bool loose_1 =  tau1->GetTauID("byLooseIsolationMVArun2v1DBoldDMwLT") ;
             bool medium_1 = tau1->GetTauID("byMediumIsolationMVArun2v1DBoldDMwLT");
             bool tight_1 =  tau1->GetTauID("byTightIsolationMVArun2v1DBoldDMwLT");
             bool vtight_1 =  tau1->GetTauID("byVTightIsolationMVArun2v1DBoldDMwLT");
             bool vvtight_1 =  tau1->GetTauID("byVVTightIsolationMVArun2v1DBoldDMwLT");
             if(gm1_ == 5){ 
              if(vloose_1 > 0.5){
                if(loose_1 < 0.5){
                  tau1_trg       = Efficiency(pt_1, 38.0333, 7.91088 , 15.20129999, 1.22660615, 0.999998);
                } else if(medium_1 < 0.5){
                  tau1_trg       = Efficiency(pt_1, 40.336709, 6.7403024, 32.5694946, 1.044272995, 0.8514444586);
                } else if(tight_1 < 0.5){
                  tau1_trg       = Efficiency(pt_1, 39.056473, 5.8055095, 4.865577978754, 2.575900, 0.999999);
                } else if(vtight_1 < 0.5){
                  tau1_trg       = Efficiency(pt_1, 38.5853083, 5.61112878,7.3018728623, 3.0092333917, 0.8600521839);
                } else if(vvtight_1 < 0.5){
                  tau1_trg       = Efficiency(pt_1, 38.618169, 5.312144619, 6.46196978, 25.920636705722, 0.8695357558);
                } else {
                  tau1_trg       = Efficiency(pt_1, 38.0330158839, 5.200365778, 6.09497263, 1.691237814,0.99999999);
                }
              }
             } else {
               if(vloose_1 > 0.5){
                if(loose_1 < 0.5){
                  tau1_trg       = Efficiency(pt_1, 42.4105036, 8.201898577, 18.314907696, 1.0362475723, 0.999996);
                } else if (medium_1<0.5){
                  tau1_trg       = Efficiency(pt_1, 40.8106337, 7.818987,10.5121936,10.9851908, 0.7664256354);
                } else if (tight_1<0.5){
                  tau1_trg       = Efficiency(pt_1, 40.17626812, 7.814412357, 10.5986531, 9.4682986699, 0.790908712);
                } else if (vtight_1<0.5){
                  tau1_trg       = Efficiency(pt_1, 40.294717, 7.869141154, 14.05910804, 1.1801018155, 0.99996473);
                } else if (vvtight_1<0.5){
                  tau1_trg      = Efficiency(pt_1, 38.639538, 7.3893785184, 11.190558468, 1.6373712, 0.818539785);
                } else {
                  tau1_trg      = Efficiency(pt_1, 39.2202518, 7.90325645, 11.32316371,1.3296217, 0.992292297);
                }
              }
             } 
             if(gm2_ == 5){ 
              if(vloose_2 > 0.5){
                if(loose_2 < 0.5){
                  tau2_trg       = Efficiency(pt_2, 38.0333, 7.91088 , 15.20129999, 1.22660615, 0.999998);
                } else if(medium_2 < 0.5){
                  tau2_trg       = Efficiency(pt_2, 40.336709, 6.7403024, 32.5694946, 1.044272995, 0.8514444586);
                } else if(tight_2 < 0.5){
                  tau2_trg       = Efficiency(pt_2, 39.056473, 5.8055095, 4.865577978754, 2.575900, 0.999999);
                } else if(vtight_2 < 0.5){
                  tau2_trg       = Efficiency(pt_2, 38.5853083, 5.61112878,7.3018728623, 3.0092333917, 0.8600521839);
                } else if(vvtight_2 < 0.5){
                  tau2_trg       = Efficiency(pt_2, 38.618169, 5.312144619, 6.46196978, 25.920636705722, 0.8695357558);
                } else {
                  tau2_trg       = Efficiency(pt_2, 38.0330158839, 5.200365778, 6.09497263, 1.691237814,0.99999999);
                }
              }
             } else {
               if(vloose_2 > 0.5){
                if(loose_2 < 0.5){
                  tau2_trg       = Efficiency(pt_2, 42.4105036, 8.201898577, 18.314907696, 1.0362475723, 0.999996);
                } else if (medium_2<0.5){
                  tau2_trg       = Efficiency(pt_2, 40.8106337, 7.818987,10.5121936,10.9851908, 0.7664256354);
                } else if (tight_2<0.5){
                  tau2_trg       = Efficiency(pt_2, 40.17626812, 7.814412357, 10.5986531, 9.4682986699, 0.790908712);
                } else if (vtight_2<0.5){
                  tau2_trg       = Efficiency(pt_2, 40.294717, 7.869141154, 14.05910804, 1.1801018155, 0.99996473);
                } else if (vvtight_2<0.5){
                  tau2_trg      = Efficiency(pt_2, 38.639538, 7.3893785184, 11.190558468, 1.6373712, 0.818539785);
                } else {
                  tau2_trg      = Efficiency(pt_2, 39.2202518, 7.90325645, 11.32316371,1.3296217, 0.992292297);
                }
              }
             } 
             /*if(gm2_ == 5){ //Using tight iso:
               tau2_trg       = Efficiency(pt_2, 38.1919, 5.38746, 4.44730, 7.39646, 9.33402e-01);
             } else {
               tau2_trg       = Efficiency(pt_2, 39.9131, 7.77317, 39.9403, 140.999, 7.84025E-01);
             }*/ 
           }else if (tt_trg_iso_mode_==0){//Using tight iso
             if(mc_ == mc::summer16_80X){
               double decay_mode_1 = tau1->decay_mode();
               double decay_mode_2 = tau2->decay_mode();
               auto args_1 = std::vector<double>{pt_1,decay_mode_1};  
               auto args_2 = std::vector<double>{pt_2,decay_mode_2};

               std::string isoWP = "Medium";
               if(strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16 || strategy_ == strategy::legacy16 || strategy_ == strategy::cpdecays16) isoWP = "Tight";
               if(gm1_ == 5){ 
                 tau1_trg = fns_["t_genuine_"+isoWP+"Iso_tt_data"]->eval(args_1.data());
                 if(is_embedded_){
                   tau1_trg_mc = fns_["t_TightIso_tt_embed"]->eval(args_1.data());
                 }
                 else tau1_trg_mc = fns_["t_genuine_"+isoWP+"Iso_tt_mc"]->eval(args_1.data());
               } else {
                 tau1_trg = fns_["t_fake_"+isoWP+"Iso_tt_data"]->eval(args_1.data());
                 if(is_embedded_) tau1_trg_mc = fns_["t_TightIso_tt_embed"]->eval(args_1.data());
                 else tau1_trg_mc = fns_["t_fake_"+isoWP+"Iso_tt_mc"]->eval(args_1.data());
               } 
               if(gm2_ == 5){ 
                 tau2_trg = fns_["t_genuine_"+isoWP+"Iso_tt_data"]->eval(args_2.data());
                 if(is_embedded_) tau2_trg_mc = fns_["t_TightIso_tt_embed"]->eval(args_2.data());
                 else tau2_trg_mc = fns_["t_genuine_"+isoWP+"Iso_tt_mc"]->eval(args_2.data());
               } else {
                 tau2_trg = fns_["t_fake_"+isoWP+"Iso_tt_data"]->eval(args_2.data());
                 if(is_embedded_) tau2_trg_mc = fns_["t_TightIso_tt_embed"]->eval(args_2.data());
                 else tau2_trg_mc = fns_["t_fake_"+isoWP+"Iso_tt_mc"]->eval(args_2.data());
               }
             } else{ 
               if(gm1_ == 5){ 
                 tau1_trg = fns_["t_trgTightIso_data"]->eval(args_1.data());
               } else {
                 tau1_trg = fns_["t_trgTightIsoSS_data"]->eval(args_1.data());
               } 
               if(gm2_ == 5){ 
                 tau2_trg = fns_["t_trgTightIso_data"]->eval(args_2.data());
               } else {
                 tau2_trg = fns_["t_trgTightIsoSS_data"]->eval(args_2.data());
               }
             }
           } else if (tt_trg_iso_mode_==1) {
             if(gm1_ == 5){ //Using medium iso
               tau1_trg       = Efficiency(pt_1, 38.2845, 5.3257, 6.450034, 1.6489256, 0.999998);
             } else {
               tau1_trg       = Efficiency(pt_1, 39.70776, 7.80895, 12.00733, 1.71461644, 0.85339127);
             } 
             if(gm2_ == 5){ //Using medium iso:
               tau2_trg       = Efficiency(pt_2, 38.2845, 5.3257, 6.450034, 1.6489256, 0.999998);
             } else {
               tau2_trg       = Efficiency(pt_2, 39.70776, 7.80895, 12.00733, 1.71461644, 0.85339127);
             } 
           } else if (tt_trg_iso_mode_==2) {
             if(gm1_ == 5){ //Using loose iso
               tau1_trg       = Efficiency(pt_1, 38.44557, 5.43466, 6.976905, 1.560941, 0.999999);
             } else {
               tau1_trg       = Efficiency(pt_1, 39.9946, 7.808277, 11.6038, 2.3037, 0.814011);
             } 
             if(gm2_ == 5){ //Using loose iso:
               tau2_trg       = Efficiency(pt_2, 38.44557, 5.43466, 6.976905, 1.560941, 0.999999);
             } else {
               tau2_trg       = Efficiency(pt_2, 39.9946, 7.808277, 11.6038, 2.3037, 0.814011);
             } 
           } else if (tt_trg_iso_mode_==3) {
             if(gm1_ == 5){ //Using vloose iso
               tau1_trg       = Efficiency(pt_1, 38.55513, 5.52419, 6.905413, 1.601455779, 0.9999999);
             } else {
               tau1_trg       = Efficiency(pt_1, 40.65517, 8.00283, 15.2985, 1.09653, 0.99999);
             } 
             if(gm2_ == 5){ //Using vloose iso:
               tau2_trg       = Efficiency(pt_2, 38.55513, 5.52419, 6.905413, 1.601455779, 0.9999999);
             } else {
               tau2_trg       = Efficiency(pt_2, 40.65517, 8.00283, 15.2985, 1.09653, 0.99999);
             }
           }
        } else if (mc_==mc::mc2017 || mc_ == mc::mc2018 || mc_ == mc::mcleg2016){
          double eta_1 = tau1->eta();
          double eta_2 = tau2->eta();  
          double phi_1 = tau1->phi();
          double phi_2 = tau2->phi();
          double dm_1 = tau1->decay_mode();
          double dm_2 = tau2->decay_mode();
          // fix for using DM10 SFs for all new DM modes too
          auto args_1 = std::vector<double>{pt_1,eta_1,phi_1,dm_1};  
          auto args_2 = std::vector<double>{pt_2,eta_2,phi_2,dm_2};

          auto args_2_1 = std::vector<double>{pt_1,dm_1};
          auto args_2_2 = std::vector<double>{pt_2,dm_2};

          double mvadm_1 = tau1->GetTauID("MVADM2017v1");
          double mvadm_2 = tau2->GetTauID("MVADM2017v1");

          auto args_mvadm_1 = std::vector<double>{pt_1,mvadm_1};
          auto args_mvadm_2 = std::vector<double>{pt_2,mvadm_2};

          double tau1_trg_ic=1., tau2_trg_ic=1., tau1_trg_ic_mvadm=1., tau2_trg_ic_mvadm=1.;

          if(is_embedded_){
            tau1_trg_mc = 1.;
            tau2_trg_mc = 1.;
            tau1_trg = fns_["t_trg_35_embed_ratio"]->eval(args_1.data());
            tau2_trg = fns_["t_trg_35_embed_ratio"]->eval(args_2.data());

            tau1_trg_ic = fns_["t_trg_35_ic_embed_ratio"]->eval(args_2_1.data());
            tau2_trg_ic = fns_["t_trg_35_ic_embed_ratio"]->eval(args_2_2.data());
            tau1_trg_ic_mvadm = fns_["t_trg_35_ic_mvadm_embed_ratio"]->eval(args_mvadm_1.data());
            tau2_trg_ic_mvadm = fns_["t_trg_35_ic_mvadm_embed_ratio"]->eval(args_mvadm_2.data());
          } else{
            tau1_trg = fns_["t_trg_35_ratio"]->eval(args_2_1.data());
            tau1_trg_mc = 1.;
            tau2_trg = fns_["t_trg_35_ratio"]->eval(args_2_2.data());
            tau2_trg_mc = 1.;

            tau1_trg_ic = fns_["t_trg_35_ic_ratio"]->eval(args_2_1.data());
            tau2_trg_ic = fns_["t_trg_35_ic_ratio"]->eval(args_2_2.data());
            tau1_trg_ic_mvadm = fns_["t_trg_35_ic_mvadm_ratio"]->eval(args_mvadm_1.data());
            tau2_trg_ic_mvadm = fns_["t_trg_35_ic_mvadm_ratio"]->eval(args_mvadm_2.data());
          }

          double tau_trg_dm0_up=1;
          double tau_trg_dm1_up=1;
          double tau_trg_dm10_up=1;
          double tau_trg_dm11_up=1;
          double tau_trg_dm0_down=1;
          double tau_trg_dm1_down=1;
          double tau_trg_dm10_down=1;
          double tau_trg_dm11_down=1;

          double tau_trg_mvadm0_up=1;
          double tau_trg_mvadm1_up=1;
          double tau_trg_mvadm2_up=1;
          double tau_trg_mvadm10_up=1;
          double tau_trg_mvadm11_up=1;
          double tau_trg_mvadm0_down=1;
          double tau_trg_mvadm1_down=1;
          double tau_trg_mvadm2_down=1;
          double tau_trg_mvadm10_down=1;
          double tau_trg_mvadm11_down=1;

          double trg_tot = (tau1_trg*tau2_trg)/(tau1_trg_mc*tau2_trg_mc);


          double tau_trg_ic = tau1_trg_ic*tau2_trg_ic;
          double tau_trg_ic_mvadm = tau1_trg_ic_mvadm*tau2_trg_ic_mvadm;

          bool do_wts = !(tau1_trg==0 || tau2_trg==0 || tau1_trg_mc==0 || tau2_trg_mc==0);

          if(is_embedded_ && do_wts){
            tau_trg_dm0_up = fns_["t_trg_35_embed_ratio_dm0_up"]->eval(args_1.data())*fns_["t_trg_35_embed_ratio_dm0_up"]->eval(args_2.data())/trg_tot;
            tau_trg_dm1_up = fns_["t_trg_35_embed_ratio_dm1_up"]->eval(args_1.data())*fns_["t_trg_35_embed_ratio_dm1_up"]->eval(args_2.data())/trg_tot;
            tau_trg_dm10_up = fns_["t_trg_35_embed_ratio_dm10_up"]->eval(args_1.data())*fns_["t_trg_35_embed_ratio_dm10_up"]->eval(args_2.data())/trg_tot;
            tau_trg_dm11_up = fns_["t_trg_35_embed_ratio_dm11_up"]->eval(args_1.data())*fns_["t_trg_35_embed_ratio_dm11_up"]->eval(args_2.data())/trg_tot;
            tau_trg_dm0_down = fns_["t_trg_35_embed_ratio_dm0_down"]->eval(args_1.data())*fns_["t_trg_35_embed_ratio_dm0_down"]->eval(args_2.data())/trg_tot;
            tau_trg_dm1_down = fns_["t_trg_35_embed_ratio_dm1_down"]->eval(args_1.data())*fns_["t_trg_35_embed_ratio_dm1_down"]->eval(args_2.data())/trg_tot;
            tau_trg_dm10_down = fns_["t_trg_35_embed_ratio_dm10_down"]->eval(args_1.data())*fns_["t_trg_35_embed_ratio_dm10_down"]->eval(args_2.data())/trg_tot;
            tau_trg_dm11_down = fns_["t_trg_35_embed_ratio_dm11_down"]->eval(args_1.data())*fns_["t_trg_35_embed_ratio_dm11_down"]->eval(args_2.data())/trg_tot;

            tau_trg_mvadm0_up = (tau_trg_ic_mvadm>0) ? fns_["t_trg_35_ic_mvadm_embed_ratio_mvadm0_up"]->eval(args_mvadm_1.data())*fns_["t_trg_35_ic_mvadm_embed_ratio_mvadm0_up"]->eval(args_mvadm_2.data())/tau_trg_ic_mvadm : 0.;
            tau_trg_mvadm1_up = (tau_trg_ic_mvadm>0) ? fns_["t_trg_35_ic_mvadm_embed_ratio_mvadm1_up"]->eval(args_mvadm_1.data())*fns_["t_trg_35_ic_mvadm_embed_ratio_mvadm1_up"]->eval(args_mvadm_2.data())/tau_trg_ic_mvadm : 0.;
            tau_trg_mvadm2_up = (tau_trg_ic_mvadm>0) ? fns_["t_trg_35_ic_mvadm_embed_ratio_mvadm2_up"]->eval(args_mvadm_1.data())*fns_["t_trg_35_ic_mvadm_embed_ratio_mvadm2_up"]->eval(args_mvadm_2.data())/tau_trg_ic_mvadm : 0.;
            tau_trg_mvadm10_up = (tau_trg_ic_mvadm>0) ? fns_["t_trg_35_ic_mvadm_embed_ratio_mvadm10_up"]->eval(args_mvadm_1.data())*fns_["t_trg_35_ic_mvadm_embed_ratio_mvadm10_up"]->eval(args_mvadm_2.data())/tau_trg_ic_mvadm : 0.;
            tau_trg_mvadm11_up = (tau_trg_ic_mvadm>0) ? fns_["t_trg_35_ic_mvadm_embed_ratio_mvadm11_up"]->eval(args_mvadm_1.data())*fns_["t_trg_35_ic_mvadm_embed_ratio_mvadm11_up"]->eval(args_mvadm_2.data())/tau_trg_ic_mvadm : 0.;

            tau_trg_mvadm0_down = (tau_trg_ic_mvadm>0) ? fns_["t_trg_35_ic_mvadm_embed_ratio_mvadm0_down"]->eval(args_mvadm_1.data())*fns_["t_trg_35_ic_mvadm_embed_ratio_mvadm0_down"]->eval(args_mvadm_2.data())/tau_trg_ic_mvadm : 0.;
            tau_trg_mvadm1_down = (tau_trg_ic_mvadm>0) ? fns_["t_trg_35_ic_mvadm_embed_ratio_mvadm1_down"]->eval(args_mvadm_1.data())*fns_["t_trg_35_ic_mvadm_embed_ratio_mvadm1_down"]->eval(args_mvadm_2.data())/tau_trg_ic_mvadm : 0.;
            tau_trg_mvadm2_down = (tau_trg_ic_mvadm>0) ? fns_["t_trg_35_ic_mvadm_embed_ratio_mvadm2_down"]->eval(args_mvadm_1.data())*fns_["t_trg_35_ic_mvadm_embed_ratio_mvadm2_down"]->eval(args_mvadm_2.data())/tau_trg_ic_mvadm : 0.;
            tau_trg_mvadm10_down = (tau_trg_ic_mvadm>0) ? fns_["t_trg_35_ic_mvadm_embed_ratio_mvadm10_down"]->eval(args_mvadm_1.data())*fns_["t_trg_35_ic_mvadm_embed_ratio_mvadm10_down"]->eval(args_mvadm_2.data())/tau_trg_ic_mvadm : 0.;
            tau_trg_mvadm11_down = (tau_trg_ic_mvadm>0) ? fns_["t_trg_35_ic_mvadm_embed_ratio_mvadm11_down"]->eval(args_mvadm_1.data())*fns_["t_trg_35_ic_mvadm_embed_ratio_mvadm11_down"]->eval(args_mvadm_2.data())/tau_trg_ic_mvadm : 0.;
          } else if(do_wts) {
            tau_trg_dm0_up = fns_["t_trg_35_ratio_dm0_up"]->eval(args_2_1.data())*fns_["t_trg_35_ratio_dm0_up"]->eval(args_2_2.data())/trg_tot;
            tau_trg_dm1_up = fns_["t_trg_35_ratio_dm1_up"]->eval(args_2_1.data())*fns_["t_trg_35_ratio_dm1_up"]->eval(args_2_2.data())/trg_tot;
            tau_trg_dm10_up = fns_["t_trg_35_ratio_dm10_up"]->eval(args_2_1.data())*fns_["t_trg_35_ratio_dm10_up"]->eval(args_2_2.data())/trg_tot;
            tau_trg_dm11_up = fns_["t_trg_35_ratio_dm11_up"]->eval(args_2_1.data())*fns_["t_trg_35_ratio_dm11_up"]->eval(args_2_2.data())/trg_tot;
            tau_trg_dm0_down = fns_["t_trg_35_ratio_dm0_down"]->eval(args_2_1.data())*fns_["t_trg_35_ratio_dm0_down"]->eval(args_2_2.data())/trg_tot;
            tau_trg_dm1_down = fns_["t_trg_35_ratio_dm1_down"]->eval(args_2_1.data())*fns_["t_trg_35_ratio_dm1_down"]->eval(args_2_2.data())/trg_tot;
            tau_trg_dm10_down = fns_["t_trg_35_ratio_dm10_down"]->eval(args_2_1.data())*fns_["t_trg_35_ratio_dm10_down"]->eval(args_2_2.data())/trg_tot;
            tau_trg_dm11_down = fns_["t_trg_35_ratio_dm11_down"]->eval(args_2_1.data())*fns_["t_trg_35_ratio_dm11_down"]->eval(args_2_2.data())/trg_tot;

            tau_trg_mvadm0_up = (tau_trg_ic_mvadm>0) ? fns_["t_trg_35_ic_mvadm_ratio_mvadm0_up"]->eval(args_mvadm_1.data())*fns_["t_trg_35_ic_mvadm_ratio_mvadm0_up"]->eval(args_mvadm_2.data())/tau_trg_ic_mvadm : 0.;
            tau_trg_mvadm1_up = (tau_trg_ic_mvadm>0) ? fns_["t_trg_35_ic_mvadm_ratio_mvadm1_up"]->eval(args_mvadm_1.data())*fns_["t_trg_35_ic_mvadm_ratio_mvadm1_up"]->eval(args_mvadm_2.data())/tau_trg_ic_mvadm : 0.;
            tau_trg_mvadm2_up = (tau_trg_ic_mvadm>0) ? fns_["t_trg_35_ic_mvadm_ratio_mvadm2_up"]->eval(args_mvadm_1.data())*fns_["t_trg_35_ic_mvadm_ratio_mvadm2_up"]->eval(args_mvadm_2.data())/tau_trg_ic_mvadm : 0.;
            tau_trg_mvadm10_up = (tau_trg_ic_mvadm>0) ? fns_["t_trg_35_ic_mvadm_ratio_mvadm10_up"]->eval(args_mvadm_1.data())*fns_["t_trg_35_ic_mvadm_ratio_mvadm10_up"]->eval(args_mvadm_2.data())/tau_trg_ic_mvadm : 0.;
            tau_trg_mvadm11_up = (tau_trg_ic_mvadm>0) ? fns_["t_trg_35_ic_mvadm_ratio_mvadm11_up"]->eval(args_mvadm_1.data())*fns_["t_trg_35_ic_mvadm_ratio_mvadm11_up"]->eval(args_mvadm_2.data())/tau_trg_ic_mvadm : 0.;

            tau_trg_mvadm0_down = (tau_trg_ic_mvadm>0) ? fns_["t_trg_35_ic_mvadm_ratio_mvadm0_down"]->eval(args_mvadm_1.data())*fns_["t_trg_35_ic_mvadm_ratio_mvadm0_down"]->eval(args_mvadm_2.data())/tau_trg_ic_mvadm : 0.;
            tau_trg_mvadm1_down = (tau_trg_ic_mvadm>0) ? fns_["t_trg_35_ic_mvadm_ratio_mvadm1_down"]->eval(args_mvadm_1.data())*fns_["t_trg_35_ic_mvadm_ratio_mvadm1_down"]->eval(args_mvadm_2.data())/tau_trg_ic_mvadm : 0.;
            tau_trg_mvadm2_down = (tau_trg_ic_mvadm>0) ? fns_["t_trg_35_ic_mvadm_ratio_mvadm2_down"]->eval(args_mvadm_1.data())*fns_["t_trg_35_ic_mvadm_ratio_mvadm2_down"]->eval(args_mvadm_2.data())/tau_trg_ic_mvadm : 0.;
            tau_trg_mvadm10_down = (tau_trg_ic_mvadm>0) ? fns_["t_trg_35_ic_mvadm_ratio_mvadm10_down"]->eval(args_mvadm_1.data())*fns_["t_trg_35_ic_mvadm_ratio_mvadm10_down"]->eval(args_mvadm_2.data())/tau_trg_ic_mvadm : 0.;
            tau_trg_mvadm11_down = (tau_trg_ic_mvadm>0) ? fns_["t_trg_35_ic_mvadm_ratio_mvadm11_down"]->eval(args_mvadm_1.data())*fns_["t_trg_35_ic_mvadm_ratio_mvadm11_down"]->eval(args_mvadm_2.data())/tau_trg_ic_mvadm : 0.;

          }

          tau_trg_dm0_up = std::isnan(tau_trg_dm0_up) ? 0 : tau_trg_dm0_up;
          tau_trg_dm0_down = std::isnan(tau_trg_dm0_down) ? 0: tau_trg_dm0_down;
          tau_trg_dm1_up = std::isnan(tau_trg_dm1_up) ? 0 : tau_trg_dm1_up;
          tau_trg_dm1_down = std::isnan(tau_trg_dm1_down) ? 0: tau_trg_dm1_down;
          tau_trg_dm10_up = std::isnan(tau_trg_dm10_up) ? 0 : tau_trg_dm10_up;
          tau_trg_dm10_down = std::isnan(tau_trg_dm10_down) ? 0: tau_trg_dm10_down;
          tau_trg_dm11_up = std::isnan(tau_trg_dm11_up) ? 0 : tau_trg_dm11_up;
          tau_trg_dm11_down = std::isnan(tau_trg_dm11_down) ? 0 : tau_trg_dm11_down;

          event->Add("wt_tau_trg_dm0_up",tau_trg_dm0_up);
          event->Add("wt_tau_trg_dm1_up",tau_trg_dm1_up);
          event->Add("wt_tau_trg_dm10_up",tau_trg_dm10_up);
          event->Add("wt_tau_trg_dm11_up",tau_trg_dm11_up);
          event->Add("wt_tau_trg_dm0_down",tau_trg_dm0_down);
          event->Add("wt_tau_trg_dm1_down",tau_trg_dm1_down);
          event->Add("wt_tau_trg_dm10_down",tau_trg_dm10_down);
          event->Add("wt_tau_trg_dm11_down",tau_trg_dm11_down);

          event->Add("wt_tau_trg_mvadm0_up",   tau_trg_mvadm0_up);
          event->Add("wt_tau_trg_mvadm1_up",   tau_trg_mvadm1_up);
          event->Add("wt_tau_trg_mvadm2_up",   tau_trg_mvadm2_up);
          event->Add("wt_tau_trg_mvadm10_up",  tau_trg_mvadm10_up);
          event->Add("wt_tau_trg_mvadm11_up",  tau_trg_mvadm11_up);
          event->Add("wt_tau_trg_mvadm0_down", tau_trg_mvadm0_down);
          event->Add("wt_tau_trg_mvadm1_down", tau_trg_mvadm1_down);
          event->Add("wt_tau_trg_mvadm2_down", tau_trg_mvadm2_down);
          event->Add("wt_tau_trg_mvadm10_down",tau_trg_mvadm10_down);
          event->Add("wt_tau_trg_mvadm11_down",tau_trg_mvadm11_down);

          tau_trg_ic = (trg_tot==0) ? tau_trg_ic : tau_trg_ic/trg_tot;
          tau_trg_ic_mvadm = (trg_tot==0) ? tau_trg_ic_mvadm : tau_trg_ic_mvadm/trg_tot;

          event->Add("wt_tau_trg_ic",tau_trg_ic);
          event->Add("wt_tau_trg_mvadm",tau_trg_ic_mvadm);
          
       }
       if(trg_applied_in_mc_){
         tau1_trg = tau1_trg / tau1_trg_mc;
         tau2_trg = tau2_trg / tau2_trg_mc;
       }
       weight *= (tau1_trg*tau2_trg);
       event->Add("trigweight_1", tau1_trg);
       event->Add("trigweight_2", tau2_trg);
       event->Add("trigweight_up_1", tau1_trg_up);
       event->Add("trigweight_up_2", tau2_trg_up);
       event->Add("trigweight_down_1", tau1_trg_down);
       event->Add("trigweight_down_2", tau2_trg_down);

       // check the trigger factorisation
       event->Add("tau1_trgeff_mc", tau1_trg_mc);
       event->Add("tau2_trgeff_mc", tau2_trg_mc);
     } else if (channel_ == channel::mtmet) {
       Muon const* muon = dynamic_cast<Muon const*>(dilepton[0]->GetCandidate("lepton1"));
       double pt = muon->pt();
       double m_eta = fabs(muon->eta());
       Tau const* tau = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton2"));
       double t_pt = tau->pt();
       double t_eta = fabs(tau->eta());
       double mu_trg = 1.0;
       double tau_trg = 1.0;
       double mu_trg_mc = 1.0;
       double tau_trg_mc = 1.0;
       if (mc_ == mc::summer12_53X) {
         double muD = 1.0;
         double mcD = 1.0;
         if (m_eta >= 0 && m_eta < 0.8) {
           muD   = Efficiency(pt, 6.9755, 0.000173944, 4.65439e-07, 1.20934, 1.35392);
         } else if (m_eta >= 0.8 && m_eta < 1.2) {
           muD   = Efficiency(pt, 7.005, 1.39453e-06, 1.77824e-11, 1.30978, 1.08927);
         } else {
           muD   = Efficiency(pt, 7.00501, 2.69916e-06, 2.54492e-11, 1.8705, 0.864745);
         } 
         mu_trg = muD;
         mu_trg_mc = mcD;
         double tdataD   = 1.0;
         double tmcD    = 1.0;
         if (fabs(t_eta) < 1.5) {
           tdataD  = Efficiency(t_pt, 18.434069, 0.530757, 0.201776, 7.190775, 0.932454);
           tmcD    = Efficiency(t_pt, 18.539244, 1.326609, 2.909963, 1.001269, 12.692931);
         } else {
           tdataD  = Efficiency(t_pt, 18.671316, 1.572636, 3.074206, 1.000632, 41.692646);
           tmcD    = Efficiency(t_pt, 18.326162, 1.849068, 3.737118, 140.684357, 0.889961);
         }
         tau_trg = tdataD;
         tau_trg_mc = tmcD;
       } 
       if (trg_applied_in_mc_) {
         mu_trg = mu_trg / mu_trg_mc;
         tau_trg = tau_trg / tau_trg_mc;
       }
       weight *= (mu_trg * tau_trg);
       event->Add("trigweight_1", mu_trg);
       event->Add("trigweight_2", tau_trg);
     } else if (channel_ == channel::zee) {
       Electron const* elec = dynamic_cast<Electron const*>(dilepton[0]->GetCandidate("lepton1"));
       Electron const* elec2 = dynamic_cast<Electron const*>(dilepton[0]->GetCandidate("lepton2"));
       double e1_pt = elec->pt();
       double e1_eta = fabs(elec->sc_eta());
       double e2_pt = elec2->pt();
       double e2_eta = fabs(elec2->sc_eta());
       double e1_signed_eta = elec->eta();
       double e2_signed_eta = elec2->eta();
       double e_iso_1 = PF03IsolationVal(elec, 0.5, 0);
       double e_iso_2 = PF03IsolationVal(elec2, 0.5, 0);
       double ele1_trg = 1.0;
       double ele2_trg = 1.0;
       double ele1_trg_mc = 1.0;
       double ele2_trg_mc = 1.0;
       if(era_ == era::data_2015 || era_==era::data_2016 || era_ == era::data_2017) e1_eta = fabs(elec->eta());
       if (mc_ == mc::fall15_76X){
         if(e1_pt<150){
           ele1_trg = et_trig_data_->GetBinContent(et_trig_data_->GetXaxis()->FindBin(e1_eta),et_trig_data_->GetYaxis()->FindBin(e1_pt));
           ele1_trg_mc = et_trig_mc_->GetBinContent(et_trig_mc_->GetXaxis()->FindBin(e1_eta),et_trig_mc_->GetYaxis()->FindBin(e1_pt));
         } else {
           ele1_trg = et_trig_data_->GetBinContent(et_trig_data_->GetXaxis()->FindBin(e1_eta),(et_trig_data_->GetYaxis()->FindBin(e1_pt)-1));
           ele1_trg_mc = et_trig_mc_->GetBinContent(et_trig_mc_->GetXaxis()->FindBin(e1_eta),(et_trig_mc_->GetYaxis()->FindBin(e1_pt)-1));
         }         

         if (e2_pt<150){
           ele2_trg = et_trig_data_->GetBinContent(et_trig_data_->GetXaxis()->FindBin(e2_eta),et_trig_data_->GetYaxis()->FindBin(e2_pt));
           ele2_trg_mc = et_trig_mc_->GetBinContent(et_trig_mc_->GetXaxis()->FindBin(e2_eta),et_trig_mc_->GetYaxis()->FindBin(e2_pt));
         } else {
           ele2_trg = et_trig_data_->GetBinContent(et_trig_data_->GetXaxis()->FindBin(e2_eta),(et_trig_data_->GetYaxis()->FindBin(e2_pt)-1));
           ele2_trg_mc = et_trig_mc_->GetBinContent(et_trig_mc_->GetXaxis()->FindBin(e2_eta),(et_trig_mc_->GetYaxis()->FindBin(e2_pt)-1));
         }
       } else if (mc_ == mc::spring16_80X || mc_ == mc::summer16_80X){
         if(scalefactor_file_==""){
             if(e1_pt<1000){
               ele1_trg = et_trig_data_->GetBinContent(et_trig_data_->GetXaxis()->FindBin(e1_eta),et_trig_data_->GetYaxis()->FindBin(e1_pt));
               ele1_trg_mc = et_trig_mc_->GetBinContent(et_trig_mc_->GetXaxis()->FindBin(e1_eta),et_trig_mc_->GetYaxis()->FindBin(e1_pt));
             } else {
               ele1_trg = et_trig_data_->GetBinContent(et_trig_data_->GetXaxis()->FindBin(e1_eta),(et_trig_data_->GetYaxis()->FindBin(e1_pt)-1));
               ele1_trg_mc = et_trig_mc_->GetBinContent(et_trig_mc_->GetXaxis()->FindBin(e1_eta),(et_trig_mc_->GetYaxis()->FindBin(e1_pt)-1));
             }         
             if(e2_pt<1000){
               ele2_trg = et_trig_data_->GetBinContent(et_trig_data_->GetXaxis()->FindBin(e2_eta),et_trig_data_->GetYaxis()->FindBin(e2_pt));
               ele2_trg_mc = et_trig_mc_->GetBinContent(et_trig_mc_->GetXaxis()->FindBin(e2_eta),et_trig_mc_->GetYaxis()->FindBin(e2_pt));
             } else {
               ele2_trg = et_trig_data_->GetBinContent(et_trig_data_->GetXaxis()->FindBin(e2_eta),(et_trig_data_->GetYaxis()->FindBin(e2_pt)-1));
               ele2_trg_mc = et_trig_mc_->GetBinContent(et_trig_mc_->GetXaxis()->FindBin(e2_eta),(et_trig_mc_->GetYaxis()->FindBin(e2_pt)-1));
             }         
         } else {
            ele1_trg = 1;
            ele1_trg_mc=1;
            ele2_trg = 1;
            ele2_trg_mc=1;
            auto args_1 = std::vector<double>{e1_pt,e1_signed_eta,e_iso_1};
            auto args_2 = std::vector<double>{e2_pt,e2_signed_eta,e_iso_2};
            auto argsdesy_1 = std::vector<double>{e1_pt,e1_signed_eta};
            auto argsdesy_2 = std::vector<double>{e2_pt,e2_signed_eta};
            if(strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16 ||   strategy_ == strategy::cpdecays16) {
                ele1_trg=fns_["e_trgEle25eta2p1WPTight_desy_data"]->eval(argsdesy_1.data());
                ele2_trg=fns_["e_trgEle25eta2p1WPTight_desy_data"]->eval(argsdesy_2.data());  
                if(e_iso_1>0.1) ele1_trg=fns_["e_trgEle25eta2p1WPTight_aiso0p1to0p3_desy_data"]->eval(argsdesy_1.data());
                if(e_iso_2>0.1) ele2_trg=fns_["e_trgEle25eta2p1WPTight_aiso0p1to0p3_desy_data"]->eval(argsdesy_2.data());     
            } else if (strategy_ == strategy::legacy16){
              ele1_trg = fns_["e_trg_binned_ratio"]->eval(args_1.data());
              ele2_trg = 1;            
            } else{
              ele1_trg = fns_["e_trg_binned_data"]->eval(args_1.data());
              ele2_trg = fns_["e_trg_binned_data"]->eval(args_2.data());
            }
            ele1_trg_mc=1;
            ele2_trg_mc=1;

            if(mc_ == mc::summer16_80X){
              if(strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16 ||  strategy_ == strategy::legacy16 || strategy_ == strategy::cpdecays16) {
                ele1_trg_mc=fns_["e_trgEle25eta2p1WPTight_desy_mc"]->eval(argsdesy_1.data());
                ele2_trg_mc=fns_["e_trgEle25eta2p1WPTight_desy_mc"]->eval(argsdesy_2.data());  
                if(e_iso_1>0.1) ele1_trg_mc=fns_["e_trgEle25eta2p1WPTight_aiso0p1to0p3_desy_mc"]->eval(argsdesy_1.data());
                if(e_iso_2>0.1) ele2_trg_mc=fns_["e_trgEle25eta2p1WPTight_aiso0p1to0p3_desy_mc"]->eval(argsdesy_2.data()); 
              } else {
                ele1_trg_mc=fns_["e_trg_binned_mc"]->eval(args_1.data());
                ele2_trg_mc=fns_["e_trg_binned_mc"]->eval(args_2.data());
              }
            }
         }
       } else if (mc_ == mc::mc2017 || mc_ == mc::mc2018 || mc_ == mc::mcleg2016){
         double e_iso_1 = PF03EAIsolationVal(elec, eventInfo->jet_rho()); //lepton_rho
         auto args_1 = std::vector<double>{e1_pt,e1_eta,e_iso_1};
         ele1_trg = fns_["e_trg_binned_data"]->eval(args_1.data());
         if(is_embedded_) ele1_trg_mc = fns_["e_trg_binned_embed"]->eval(args_1.data());
         else ele1_trg_mc = fns_["e_trg_binned_mc"]->eval(args_1.data());
         ele2_trg = 1.0;
         ele2_trg_mc = 1.0;
       } else if(mc_ != mc::summer16_80X){
         ele1_trg = 1-((1-ele1_trg)*(1-ele2_trg));
         ele1_trg_mc = 1-((1-ele1_trg_mc)*(1-ele2_trg_mc));
       } 
       // for 16/17/18 only allow first electron to fire trigger
       ele2_trg = 1.0;
       ele2_trg_mc = 1.0;
       if (trg_applied_in_mc_) {
         ele1_trg = ele1_trg / ele1_trg_mc;
         ele2_trg = ele2_trg / ele2_trg_mc;
       }
       weight *= (ele1_trg * ele2_trg);
       event->Add("trigweight_1", ele1_trg);
       event->Add("trigweight_2", ele2_trg);
     
     } else if (channel_ == channel::zmm) {
       Muon const* muon = dynamic_cast<Muon const*>(dilepton[0]->GetCandidate("lepton1"));
       Muon const* muon2 = dynamic_cast<Muon const*>(dilepton[0]->GetCandidate("lepton2"));
       double pt1 = muon->pt();
       double pt2 = muon2->pt();
       double m1_eta = fabs(muon->eta());
       double m2_eta = fabs(muon2->eta());
       double m1_signed_eta = muon->eta();
       double m2_signed_eta = muon2->eta();
       double m_iso_1 = PF04IsolationVal(muon, 0.5, 0);
       double m_iso_2 = PF04IsolationVal(muon2, 0.5, 0);
       double mu1_trg = 1.0;
       double mu2_trg = 1.0;
       double mu1_trg_mc = 1.0;
       double mu2_trg_mc = 1.0;
       if (mc_ == mc::fall15_76X){
         if(pt1<150){
           mu1_trg = mt_trig_data_->GetBinContent(mt_trig_data_->GetXaxis()->FindBin(m1_eta),mt_trig_data_->GetYaxis()->FindBin(pt1));
           mu1_trg_mc = mt_trig_mc_->GetBinContent(mt_trig_mc_->GetXaxis()->FindBin(m1_eta),mt_trig_mc_->GetYaxis()->FindBin(pt1));
         } else {
           mu1_trg = mt_trig_data_->GetBinContent(mt_trig_data_->GetXaxis()->FindBin(m1_eta),(mt_trig_data_->GetYaxis()->FindBin(pt1)-1));
           mu1_trg_mc = mt_trig_mc_->GetBinContent(mt_trig_mc_->GetXaxis()->FindBin(m1_eta),(mt_trig_mc_->GetYaxis()->FindBin(pt1)-1));
         }         

         if(pt2<150){
           mu2_trg = mt_trig_data_->GetBinContent(mt_trig_data_->GetXaxis()->FindBin(m2_eta),mt_trig_data_->GetYaxis()->FindBin(pt2));
           mu2_trg_mc = mt_trig_mc_->GetBinContent(mt_trig_mc_->GetXaxis()->FindBin(m2_eta),mt_trig_mc_->GetYaxis()->FindBin(pt2));
         } else {
           mu2_trg = mt_trig_data_->GetBinContent(mt_trig_data_->GetXaxis()->FindBin(m2_eta),(mt_trig_data_->GetYaxis()->FindBin(pt2)-1));
           mu2_trg_mc = mt_trig_mc_->GetBinContent(mt_trig_mc_->GetXaxis()->FindBin(m2_eta),(mt_trig_mc_->GetYaxis()->FindBin(pt2)-1));
         }
        } else if(mc_ == mc::spring16_80X || mc_ == mc::summer16_80X){
         if(scalefactor_file_==""){
          if(pt1<1000){
             mu1_trg = mt_trig_data_->GetBinContent(mt_trig_data_->GetXaxis()->FindBin(m1_eta),mt_trig_data_->GetYaxis()->FindBin(pt1));
             mu1_trg_mc = mt_trig_mc_->GetBinContent(mt_trig_mc_->GetXaxis()->FindBin(m1_eta),mt_trig_mc_->GetYaxis()->FindBin(pt1));
           } else {
             mu1_trg = mt_trig_data_->GetBinContent(mt_trig_data_->GetXaxis()->FindBin(m1_eta),(mt_trig_data_->GetYaxis()->FindBin(pt1)-1));
             mu1_trg_mc = mt_trig_mc_->GetBinContent(mt_trig_mc_->GetXaxis()->FindBin(m1_eta),(mt_trig_mc_->GetYaxis()->FindBin(pt1)-1));
           }         
           if(pt2<1000){
             mu2_trg = mt_trig_data_->GetBinContent(mt_trig_data_->GetXaxis()->FindBin(m2_eta),mt_trig_data_->GetYaxis()->FindBin(pt2));
             mu2_trg_mc = mt_trig_data_->GetBinContent(mt_trig_data_->GetXaxis()->FindBin(m2_eta),mt_trig_data_->GetYaxis()->FindBin(pt2));
           } else {
             mu2_trg = mt_trig_data_->GetBinContent(mt_trig_data_->GetXaxis()->FindBin(m2_eta),(mt_trig_data_->GetYaxis()->FindBin(pt2)-1));
             mu2_trg_mc = mt_trig_mc_->GetBinContent(mt_trig_mc_->GetXaxis()->FindBin(m2_eta),(mt_trig_mc_->GetYaxis()->FindBin(pt2)-1));
           }

         } else {
            mu1_trg = 1;
            mu1_trg_mc=1;
            mu2_trg = 1;
            mu2_trg_mc=1;
            auto args_1 = std::vector<double>{pt1,m1_signed_eta,m_iso_1};
            auto args_2 = std::vector<double>{pt2,m2_signed_eta,m_iso_2};
            
            if(mc_ == mc::summer16_80X && strategy_ != strategy::smsummer16 && strategy_ != strategy::cpsummer16 &&  strategy_ != strategy::legacy16 && strategy_ != strategy::cpdecays16){
              mu1_trg = fns_["m_trgOR4_binned_data"]->eval(args_1.data()); 
              mu2_trg = fns_["m_trgOR4_binned_data"]->eval(args_2.data());
              mu1_trg_mc=fns_["m_trgOR4_binned_mc"]->eval(args_1.data());
              mu2_trg_mc=fns_["m_trgOR4_binned_mc"]->eval(args_2.data());
            } else if (strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16 || strategy_ == strategy::cpdecays16){
              if (is_embedded_) {
                mu1_trg = fns_["m_trg_binned_data"]->eval(args_1.data()); 
                mu2_trg = 1;
                mu1_trg_mc=fns_["m_trg_binned_mc"]->eval(args_1.data());
                mu2_trg_mc=1;    
              } else {   
                auto argsdesy_1 = std::vector<double>{pt1,m1_signed_eta};
                auto argsdesy_2 = std::vector<double>{pt2,m2_signed_eta};
                mu1_trg = fns_["m_trgMu22OR_eta2p1_desy_data"]->eval(argsdesy_1.data()); 
                mu2_trg = fns_["m_trgMu22OR_eta2p1_desy_data"]->eval(argsdesy_2.data());
                mu1_trg_mc=fns_["m_trgMu22OR_eta2p1_desy_mc"]->eval(argsdesy_1.data());
                mu2_trg_mc=fns_["m_trgMu22OR_eta2p1_desy_mc"]->eval(argsdesy_2.data());
                if(m_iso_1>0.15){
                  mu1_trg = fns_["m_trgMu22OR_eta2p1_aiso0p15to0p3_desy_data"]->eval(argsdesy_1.data()); 
                  mu1_trg_mc=fns_["m_trgMu22OR_eta2p1_aiso0p15to0p3_desy_mc"]->eval(argsdesy_1.data());
                }
                if(m_iso_2>0.15){
                  mu2_trg = fns_["m_trgMu22OR_eta2p1_aiso0p15to0p3_desy_data"]->eval(argsdesy_2.data());
                  mu2_trg_mc=fns_["m_trgMu22OR_eta2p1_aiso0p15to0p3_desy_mc"]->eval(argsdesy_2.data());
                }
              }
              
            } else if (strategy_ == strategy::legacy16){            
              mu1_trg = fns_["m_trg_binned_ratio"]->eval(args_1.data());
              mu2_trg = 1;
              mu1_trg_mc=1;
              mu2_trg_mc=1;                        
            } else{
              mu1_trg = fns_["m_trgOR_binned_data"]->eval(args_1.data());
              mu2_trg = fns_["m_trgOR_binned_data"]->eval(args_2.data());
              mu1_trg_mc=1;
              mu2_trg_mc=1;   
            }
          }
       } else if (mc_ == mc::mc2018 || mc_ == mc::mc2017 || mc_ == mc::mcleg2016){
         auto args_1 = std::vector<double>{pt1,m1_signed_eta,m_iso_1};  
         auto args_2 = std::vector<double>{pt1,m1_signed_eta};
         mu1_trg = fns_["m_trg_binned_data"]->eval(args_1.data());
         if (is_embedded_) {
           mu1_trg_mc = fns_["m_trg_binned_embed"]->eval(args_1.data());
         } else {
           mu1_trg_mc = fns_["m_trg_binned_mc"]->eval(args_1.data());
         }
         mu2_trg = 1.0;
         mu2_trg_mc = 1.0;
       }
       else if(mc_ != mc::summer16_80X){
         mu1_trg = 1-((1-mu1_trg)*(1-mu2_trg));
         mu1_trg_mc = 1-((1-mu1_trg_mc)*(1-mu2_trg_mc));
       }
       // for summer16 only allow lead muon to fire trigger
       mu2_trg = 1.0;
       mu2_trg_mc = 1.0;
 
       if (trg_applied_in_mc_) {
         mu1_trg = mu1_trg / mu1_trg_mc;
         mu2_trg = mu2_trg / mu2_trg_mc;
       }
       weight *= (mu1_trg * mu2_trg);
       event->Add("trigweight_1", mu1_trg);
       event->Add("trigweight_2", mu2_trg);
     }
   }
   if (do_singlemu_trg_weights_) {
       Muon const* muon = dynamic_cast<Muon const*>(dilepton[0]->GetCandidate("lepton1"));
       double m_pt = muon->pt();
       double m_eta = fabs(muon->eta());
       double m_trg = 1.0;
       double m_trg_mc = 1.0;
       if (m_eta < 0.9) {
         if (m_pt > 25.0)                { m_trg_mc = 0.93125; m_trg = 0.91403; }
       } else if (m_eta >= 0.9 && m_eta < 1.2) {
         if (m_pt > 25.0)                { m_trg_mc = 0.85067; m_trg = 0.81797; }
       } else {
         if (m_pt > 25.0)                { m_trg_mc = 0.80169; m_trg = 0.79945; }
       }
       if (trg_applied_in_mc_) m_trg = m_trg / m_trg_mc;
       weight *= (m_trg);
       event->Add("trigweight_1", m_trg);
   }

   if (do_idiso_weights_ || do_id_weights_) {
     if (channel_ == channel::et) {
       Electron const* elec = dynamic_cast<Electron const*>(dilepton[0]->GetCandidate("lepton1"));
       double pt = elec->pt();
       double sc_eta = fabs(elec->sc_eta());
       double e_signed_eta = elec->sc_eta();
       double e_iso = PF03IsolationVal(elec, 0.5, 0);
       if(era_ == era::data_2015||era_ == era::data_2016 || era_ == era::data_2017) sc_eta = fabs(elec->eta());
       double ele_id = 1.0;
       double ele_iso = 1.0;
       double ele_idiso_mc =1.0;
       double ele_idiso_data =1.0;
       double ele_idiso=1.0;
       if (mc_ == mc::summer12_53X) {
         if (era_ == era::data_2012_rereco) {
           if (pt > 24.0 && pt <= 30.0 && sc_eta < 1.479)  { ele_id = 0.8999; ele_iso = 0.9417; }
           if (pt > 24.0 && pt <= 30.0 && sc_eta >= 1.479) { ele_id = 0.7945; ele_iso = 0.9471; }
           if (pt > 30.0 && sc_eta < 1.479)                { ele_id = 0.9486; ele_iso = 0.9804; }
           if (pt > 30.0 && sc_eta >= 1.479)               { ele_id = 0.8866; ele_iso = 0.9900; }  
         }
       } else if (mc_ == mc::fall11_42X) {    
         if (pt > 20.0 && pt <= 30.0 && sc_eta < 1.479)  { ele_id = 0.9590; ele_iso = 0.9907; }
         if (pt > 20.0 && pt <= 30.0 && sc_eta >= 1.479) { ele_id = 0.9462; ele_iso = 0.9875; }
         if (pt > 30.0 && sc_eta < 1.479)                { ele_id = 0.9826; ele_iso = 0.9845; }
         if (pt > 30.0 && sc_eta >= 1.479)               { ele_id = 0.9689; ele_iso = 0.9971; }
       } else if (mc_ == mc::spring15_74X || mc_ ==mc::fall15_76X){
         if(pt<100){
           ele_idiso_data = et_idiso_data_->GetBinContent(et_idiso_data_->GetXaxis()->FindBin(sc_eta),et_idiso_data_->GetYaxis()->FindBin(pt));
           ele_idiso_mc = et_idiso_mc_->GetBinContent(et_idiso_mc_->GetXaxis()->FindBin(sc_eta),et_idiso_mc_->GetYaxis()->FindBin(pt));
         } else {
           ele_idiso_data = et_idiso_data_->GetBinContent(et_idiso_data_->GetXaxis()->FindBin(sc_eta),(et_idiso_data_->GetYaxis()->FindBin(pt)-1));
           ele_idiso_mc = et_idiso_mc_->GetBinContent(et_idiso_mc_->GetXaxis()->FindBin(sc_eta),(et_idiso_mc_->GetYaxis()->FindBin(pt)-1));
         }         
           ele_idiso = ele_idiso_data/ele_idiso_mc;

       } else if (mc_ == mc::spring16_80X){
          auto args_1 = std::vector<double>{pt,e_signed_eta};
          auto args_2 = std::vector<double>{pt,e_signed_eta,e_iso};
          if(e_iso < 0.1){
            ele_idiso = fns_["e_idiso0p10_desy_ratio"]->eval(args_1.data());
          } else ele_idiso = fns_["e_id_ratio"]->eval(args_1.data()) * fns_["e_iso_binned_ratio"]->eval(args_2.data()) ;
       } else if (mc_ == mc::summer16_80X && strategy_ != strategy::smsummer16 && strategy_ != strategy::cpsummer16 &&   strategy_ != strategy::cpdecays16 && !is_embedded_){
          auto args_1 = std::vector<double>{pt,e_signed_eta};
          auto args_2 = std::vector<double>{pt,e_signed_eta,e_iso};
          ele_idiso = fns_["e_id_ratio"]->eval(args_1.data()) * fns_["e_iso_binned_ratio"]->eval(args_2.data()) ;
       } else if (mc_ == mc::summer16_80X && (strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16  ||  strategy_ == strategy::cpdecays16) && !is_embedded_){
          auto args_1 = std::vector<double>{pt,e_signed_eta};
          ele_idiso = fns_["e_idiso0p1_desy_ratio"]->eval(args_1.data());
          if(e_iso>0.1) ele_idiso = fns_["e_idiso_aiso0p1to0p3_desy_ratio"]->eval(args_1.data());
       } else if (mc_ == mc::summer16_80X&&is_embedded_) {
         auto args_1 = std::vector<double>{pt,e_signed_eta};
         auto args_2 = std::vector<double>{pt,e_signed_eta,e_iso};
         ele_idiso = fns_["e_id_ratio"]->eval(args_1.data());
         double e_iso_wt = 1.0;
         e_iso_wt = fns_["e_iso_binned_ratio"]->eval(args_2.data());
         ele_idiso*=e_iso_wt;
       } else if(mc_==mc::mc2017 || mc_ == mc::mc2018 || mc_ == mc::mcleg2016){
         e_iso = PF03EAIsolationVal(elec, eventInfo->jet_rho()); 
         double e_sceta = elec->sc_eta();
         auto args_1 = std::vector<double>{pt,e_sceta,e_iso};  
         if(is_embedded_){
           ele_idiso = fns_["e_idiso_embed_ratio"]->eval(args_1.data());
         } else {
           ele_idiso = fns_["e_idiso_ratio"]->eval(args_1.data());
           //std::cout << ele_idiso << std::endl; 
         }
         eventInfo->set_weight("idiso", ele_idiso, false);
       }
       if(mc_==mc::mc2017 || mc_ == mc::mc2018 || mc_ == mc::mcleg2016){
         event->Add("idisoweight_1",ele_idiso);
         //event->Add("idisoweight_2",double(1.0));
         weight *= (ele_idiso); 
       } else if(mc_ != mc::spring15_74X && mc_ != mc::fall15_76X && mc_!=mc::spring16_80X && mc_ != mc::summer16_80X){
         if (do_id_weights_) ele_iso = 1.0;
         weight *= (ele_id * ele_iso);
         event->Add("idweight_1", ele_id);
         event->Add("idweight_2", double(1.0));
         event->Add("isoweight_1", ele_iso);
         event->Add("isoweight_2", double(1.0));
       } else {
         weight *= ele_idiso;
         event->Add("idisoweight_1",ele_idiso);
         event->Add("idisoweight_2",double(1.0));
       }
     } else if (channel_ == channel::mt) {
       Muon const* muon = dynamic_cast<Muon const*>(dilepton[0]->GetCandidate("lepton1"));
       double pt = muon->pt();
       double m_eta = fabs(muon->eta());
       double m_signed_eta = muon->eta();
       double m_iso = PF04IsolationVal(muon, 0.5, 0);
       double mu_id = 1.0;
       double mu_iso = 1.0;
       double mu_idiso_mc = 1.0;
       double mu_idiso_data = 1.0;
       double mu_idiso=1.0;
       if (mc_ == mc::summer12_53X) {
         if (era_ == era::data_2012_rereco) {
           if (pt > 20.0 && pt <= 30.0 && m_eta < 0.8)                   { mu_id = 0.9818; mu_iso = 0.9494; }
           if (pt > 20.0 && pt <= 30.0 && m_eta >= 0.8 && m_eta < 1.2)   { mu_id = 0.9829; mu_iso = 0.9835; }
           if (pt > 20.0 && pt <= 30.0 && m_eta >= 1.2)                  { mu_id = 0.9869; mu_iso = 0.9923; }
           if (pt > 30.0 && m_eta < 0.8)                                 { mu_id = 0.9852; mu_iso = 0.9883; }
           if (pt > 30.0 && m_eta >= 0.8 && m_eta < 1.2)                 { mu_id = 0.9850; mu_iso = 0.9937; }
           if (pt > 30.0 && m_eta >= 1.2)                                { mu_id = 0.9884; mu_iso = 0.9996; }
         }
       } else if (mc_ == mc::fall11_42X) {
         if (pt > 17.0 && pt <= 20.0 && m_eta < 0.8)                   { mu_id = 0.9963; mu_iso = 0.9910; }
         if (pt > 17.0 && pt <= 20.0 && m_eta >= 0.8 && m_eta < 1.2)   { mu_id = 0.9846; mu_iso = 0.9643; }
         if (pt > 17.0 && pt <= 20.0 && m_eta >= 1.2)                  { mu_id = 0.9830; mu_iso = 0.9504; }
         if (pt > 20.0 && pt <= 30.0 && m_eta < 0.8)                   { mu_id = 0.9962; mu_iso = 1.0011; }
         if (pt > 20.0 && pt <= 30.0 && m_eta >= 0.8 && m_eta < 1.2)   { mu_id = 0.9904; mu_iso = 0.9834; }
         if (pt > 20.0 && pt <= 30.0 && m_eta >= 1.2)                  { mu_id = 0.9828; mu_iso = 0.9975; }
         if (pt > 30.0 && m_eta < 0.8)                                 { mu_id = 0.9977; mu_iso = 0.9895; }
         if (pt > 30.0 && m_eta >= 0.8 && m_eta < 1.2)                 { mu_id = 0.9893; mu_iso = 0.9936; }
         if (pt > 30.0 && m_eta >= 1.2)                                { mu_id = 0.9829; mu_iso = 0.9960; }
       } else if (mc_ == mc::spring15_74X ||mc_ == mc::fall15_76X){
         if(pt<100){
           mu_idiso_data = mt_idiso_data_->GetBinContent(mt_idiso_data_->GetXaxis()->FindBin(m_eta),mt_idiso_data_->GetYaxis()->FindBin(pt));
           mu_idiso_mc = mt_idiso_mc_->GetBinContent(mt_idiso_mc_->GetXaxis()->FindBin(m_eta),mt_idiso_mc_->GetYaxis()->FindBin(pt));
         } else {
           mu_idiso_data = mt_idiso_data_->GetBinContent(mt_idiso_data_->GetXaxis()->FindBin(m_eta),(mt_idiso_data_->GetYaxis()->FindBin(pt)-1));
           mu_idiso_mc = mt_idiso_mc_->GetBinContent(mt_idiso_mc_->GetXaxis()->FindBin(m_eta),(mt_idiso_mc_->GetYaxis()->FindBin(pt)-1));
         }         
           mu_idiso = mu_idiso_data/mu_idiso_mc;

       } else if(mc_ == mc::spring16_80X){
          auto args_1 = std::vector<double>{pt,m_signed_eta};
          auto args_2 = std::vector<double>{pt,m_signed_eta,m_iso};
          if(m_iso<0.15){
            mu_idiso = fns_["m_idiso0p15_desy_ratio"]->eval(args_1.data());
          } else mu_idiso = fns_["m_id_ratio"]->eval(args_1.data()) * fns_["m_iso_binned_ratio"]->eval(args_2.data()) ;
       } else if(mc_ == mc::summer16_80X && strategy_ != strategy::smsummer16 && strategy_ != strategy::cpsummer16 &&  strategy_ != strategy::legacy16 && strategy_ != strategy::cpdecays16  && !is_embedded_){
          auto args_1 = std::vector<double>{pt,m_signed_eta};
          auto args_2 = std::vector<double>{pt,m_signed_eta,m_iso};
          mu_idiso = fns_["m_id_ratio"]->eval(args_1.data()) * fns_["m_iso_binned_ratio"]->eval(args_2.data());
       } else if(mc_ == mc::summer16_80X && is_embedded_){
          auto args_1 = std::vector<double>{pt,m_signed_eta};
          auto args_2 = std::vector<double>{pt,m_signed_eta,m_iso};
          mu_idiso = fns_["m_id_ratio"]->eval(args_1.data());
          double mu_iso = 1.0;
          mu_iso = fns_["m_iso_binned_ratio"]->eval(args_2.data());
          mu_idiso*=mu_iso;
       } else if(mc_ == mc::summer16_80X && (strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16 || strategy_ == strategy::cpdecays16) && !is_embedded_){
          auto args_1 = std::vector<double>{pt,m_signed_eta};
          mu_idiso = fns_["m_idiso0p15_desy_ratio"]->eval(args_1.data());
          if(m_iso>0.15) mu_idiso = fns_["m_idiso_aiso0p15to0p3_desy_ratio"]->eval(args_1.data());
       /*} else if (strategy_ == strategy::legacy16){ 
           auto args_1 = std::vector<double>{pt,m_signed_eta};
           auto args_2 = std::vector<double>{pt,m_signed_eta,m_iso};
           mu_id = fns_["m_id_ratio"]->eval(args_1.data());
           mu_iso = fns_["m_iso_binned_ratio"]->eval(args_2.data());
           mu_idiso = mu_id * mu_iso;*/
       } else if (mc_==mc::mc2017 || mc_ == mc::mc2018 || mc_ == mc::mcleg2016){
         auto args_1 = std::vector<double>{pt,m_signed_eta,m_iso};  
         if(is_embedded_){
           mu_idiso = fns_["m_idiso_embed_ratio"]->eval(args_1.data());
         } else{
           mu_idiso = fns_["m_idiso_ratio"]->eval(args_1.data());
         }

       } 
       if(mc_==mc::mc2017 || mc_ == mc::mc2018 || mc_ == mc::mcleg2016) {
         weight *= mu_idiso;
         event->Add("idisoweight_1", mu_idiso);
         //event->Add("idisoweight_2", double(1.0));  
         event->Add("idweight_1", mu_id);
         event->Add("idweight_2", double(1.0));
         event->Add("isoweight_1", mu_iso);
         event->Add("isoweight_2", double(1.0));
       } else if(mc_ != mc::spring15_74X && mc_ != mc::fall15_76X && mc_ != mc::spring16_80X && mc_ != mc::summer16_80X){ 
         if (do_id_weights_) mu_iso = 1.0;
         weight *= (mu_id * mu_iso);
         event->Add("idweight_1", mu_id);
         event->Add("idweight_2", double(1.0));
         event->Add("isoweight_1", mu_iso);
         event->Add("isoweight_2", double(1.0));
       } else {
         weight *= mu_idiso;
         event->Add("idisoweight_1", mu_idiso);
         event->Add("idisoweight_2", double(1.0));
       }
     } else if (channel_ == channel::em) {
       Electron const* elec = dynamic_cast<Electron const*>(dilepton[0]->GetCandidate("lepton1"));
       Muon const* muon = dynamic_cast<Muon const*>(dilepton[0]->GetCandidate("lepton2"));

       unsigned gm1_ = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_1"));
       unsigned gm2_ = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_2"));
       double e_pt = elec->pt();
       double e_eta = fabs(elec->sc_eta());
       double e_signed_eta = elec->eta();
       if (era_ == era::data_2015 || era_ == era::data_2016 || era_ == era::data_2017) e_eta = fabs(elec->eta());
       double m_pt = muon->pt();
       double m_eta = fabs(muon->eta());
       double m_signed_eta = muon->eta();
       double m_idiso = 1.0;
       double m_idiso_up = 1.0;
       double m_idiso_down = 1.0;
       double e_idiso = 1.0;
       double e_idiso_up = 1.0;
       double e_idiso_down = 1.0;
       double m_idiso_mc = 1.0;
       double m_idiso_data = 1.0;
       double e_idiso_mc = 1.0;
       double e_idiso_data = 1.0;
       if (mc_ == mc::summer12_53X) {
         if (era_ == era::data_2012_rereco) {
           if (m_eta < 0.8) {
             if (m_pt <= 15.0)                 m_idiso = 0.9771;
             if (m_pt > 15.0 && m_pt <= 20.0)  m_idiso = 0.9548;
             if (m_pt > 20.0 && m_pt <= 25.0)  m_idiso = 0.9648;
             if (m_pt > 25.0 && m_pt <= 30.0)  m_idiso = 0.9676;
             if (m_pt > 30.0 && m_pt <= 35.0)  m_idiso = 0.9730;
             if (m_pt > 35.0)                  m_idiso = 0.9826;
           } else if (m_eta >= 0.8 && m_eta < 1.2) {
             if (m_pt <= 15.0)                 m_idiso = 0.9746;
             if (m_pt > 15.0 && m_pt <= 20.0)  m_idiso = 0.9701;
             if (m_pt > 20.0 && m_pt <= 25.0)  m_idiso = 0.9836;
             if (m_pt > 25.0 && m_pt <= 30.0)  m_idiso = 0.9817;
             if (m_pt > 30.0 && m_pt <= 35.0)  m_idiso = 0.9833;
             if (m_pt > 35.0)                  m_idiso = 0.9841;
           } else if (m_eta >= 1.2 && m_eta < 1.6) {
             if (m_pt <= 15.0)                 m_idiso = 0.9644;
             if (m_pt > 15.0 && m_pt <= 20.0)  m_idiso = 0.9766;
             if (m_pt > 20.0 && m_pt <= 25.0)  m_idiso = 0.9820;
             if (m_pt > 25.0 && m_pt <= 30.0)  m_idiso = 0.9886;
             if (m_pt > 30.0 && m_pt <= 35.0)  m_idiso = 0.9910;
             if (m_pt > 35.0)                  m_idiso = 0.9900;
           } else {
             if (m_pt <= 15.0)                 m_idiso = 0.9891;
             if (m_pt > 15.0 && m_pt <= 20.0)  m_idiso = 0.9892;
             if (m_pt > 20.0 && m_pt <= 25.0)  m_idiso = 0.9909;
             if (m_pt > 25.0 && m_pt <= 30.0)  m_idiso = 0.9883;
             if (m_pt > 30.0 && m_pt <= 35.0)  m_idiso = 0.9900;
             if (m_pt > 35.0)                  m_idiso = 0.9886;
           }
           if (e_eta < 0.8) {
             if (e_pt <= 15.0)                 e_idiso = 0.7654;
             if (e_pt > 15.0 && e_pt <= 20.0)  e_idiso = 0.8394;
             if (e_pt > 20.0 && e_pt <= 25.0)  e_idiso = 0.8772;
             if (e_pt > 25.0 && e_pt <= 30.0)  e_idiso = 0.9006;
             if (e_pt > 30.0 && e_pt <= 35.0)  e_idiso = 0.9261;
             if (e_pt > 35.0)                  e_idiso = 0.9514;
           } else if (e_eta >= 0.8 && e_eta < 1.479) {
             if (e_pt <= 15.0)                 e_idiso = 0.7693;
             if (e_pt > 15.0 && e_pt <= 20.0)  e_idiso = 0.8457;
             if (e_pt > 20.0 && e_pt <= 25.0)  e_idiso = 0.8530;
             if (e_pt > 25.0 && e_pt <= 30.0)  e_idiso = 0.8874;
             if (e_pt > 30.0 && e_pt <= 35.0)  e_idiso = 0.9199;
             if (e_pt > 35.0)                  e_idiso = 0.9445;
           } else {
             if (e_pt <= 15.0)                 e_idiso = 0.5719;
             if (e_pt > 15.0 && e_pt <= 20.0)  e_idiso = 0.7024;
             if (e_pt > 20.0 && e_pt <= 25.0)  e_idiso = 0.7631;
             if (e_pt > 25.0 && e_pt <= 30.0)  e_idiso = 0.8092;
             if (e_pt > 30.0 && e_pt <= 35.0)  e_idiso = 0.8469;
             if (e_pt > 35.0)                  e_idiso = 0.9078;
           }
         }
       } else if (mc_ == mc::fall11_42X) {
         if (m_eta < 0.8) {
           if (m_pt <= 15.0)                 m_idiso = 0.9303;
           if (m_pt > 15.0 && m_pt <= 20.0)  m_idiso = 1.0176;
           if (m_pt > 20.0)                  m_idiso = 0.9998;
         } else if (m_eta >= 0.8 && m_eta < 1.2) {
           if (m_pt <= 15.0)                 m_idiso = 1.0125;
           if (m_pt > 15.0 && m_pt <= 20.0)  m_idiso = 1.0040;
           if (m_pt > 20.0)                  m_idiso = 1.0006;
         } else {
           if (m_pt <= 15.0)                 m_idiso = 0.9994;
           if (m_pt > 15.0 && m_pt <= 20.0)  m_idiso = 1.0063;
           if (m_pt > 20.0)                  m_idiso = 1.0045;
         }
         if (e_eta < 0.8) {
           if (e_pt <= 15.0)                 e_idiso = 1.0078;
           if (e_pt > 15.0 && e_pt <= 20.0)  e_idiso = 0.9612;
           if (e_pt > 20.0)                  e_idiso = 0.9862;
         } else if (e_eta >= 0.8 && e_eta < 1.479) {
           if (e_pt <= 15.0)                 e_idiso = 1.1236;
           if (e_pt > 15.0 && e_pt <= 20.0)  e_idiso = 0.9773;
           if (e_pt > 20.0)                  e_idiso = 0.9786;
         } else {
           if (e_pt <= 15.0)                 e_idiso = 0.9336;
           if (e_pt > 15.0 && e_pt <= 20.0)  e_idiso = 1.0600;
           if (e_pt > 20.0)                  e_idiso = 1.0136;  
         }
       } else if (mc_ == mc::spring15_74X || mc_ ==mc::fall15_76X){
         if(m_pt<100){
           m_idiso_data = em_m_idiso_data_->GetBinContent(em_m_idiso_data_->GetXaxis()->FindBin(m_eta),em_m_idiso_data_->GetYaxis()->FindBin(m_pt));
           m_idiso_mc = em_m_idiso_mc_->GetBinContent(em_m_idiso_mc_->GetXaxis()->FindBin(m_eta),em_m_idiso_mc_->GetYaxis()->FindBin(m_pt));
         } else {
           m_idiso_data = em_m_idiso_data_->GetBinContent(em_m_idiso_data_->GetXaxis()->FindBin(m_eta),(em_m_idiso_data_->GetYaxis()->FindBin(m_pt)-1));
           m_idiso_mc = em_m_idiso_mc_->GetBinContent(em_m_idiso_mc_->GetXaxis()->FindBin(m_eta),(em_m_idiso_mc_->GetYaxis()->FindBin(m_pt)-1));
         }         
         if(e_pt<100){
           e_idiso_data = em_e_idiso_data_->GetBinContent(em_e_idiso_data_->GetXaxis()->FindBin(e_eta),em_e_idiso_data_->GetYaxis()->FindBin(e_pt));
           e_idiso_mc = em_e_idiso_mc_->GetBinContent(em_e_idiso_mc_->GetXaxis()->FindBin(e_eta),em_e_idiso_mc_->GetYaxis()->FindBin(e_pt));
         } else {
           e_idiso_data = em_e_idiso_data_->GetBinContent(em_e_idiso_data_->GetXaxis()->FindBin(e_eta),(em_e_idiso_data_->GetYaxis()->FindBin(e_pt)-1));
           e_idiso_mc = em_e_idiso_mc_->GetBinContent(em_e_idiso_mc_->GetXaxis()->FindBin(e_eta),(em_e_idiso_mc_->GetYaxis()->FindBin(e_pt)-1));
         }         

           m_idiso = m_idiso_data/m_idiso_mc;
           e_idiso = e_idiso_data/e_idiso_mc;

       } else if (mc_ == mc::spring16_80X){
          auto args_1 = std::vector<double>{m_pt,m_signed_eta};
          m_idiso = fns_["m_idiso0p20_desy_ratio"]->eval(args_1.data()) ;
          
          auto args_2 = std::vector<double>{e_pt,e_signed_eta};
          e_idiso = fns_["e_idiso0p15_desy_ratio"]->eval(args_2.data()) ;

        } else if(mc_ == mc::summer16_80X){
           double m_iso = PF04IsolationVal(muon, 0.5, 0); 
           double e_iso = PF03IsolationVal(elec, 0.5, 0);             
           std::vector<PFJet*> uncleaned_jets = event->GetPtrVec<PFJet>("ak4PFJetsCHSUnFiltered");
           if(gm2_<5 || !do_jlepton_fake_){
             auto args_1_2 = std::vector<double>{m_pt,m_signed_eta};
             auto args_2_2 = std::vector<double>{m_pt,m_signed_eta,m_iso};
             if(!is_embedded_){
                m_idiso=fns_["m_idiso0p20_desy_ratio"]->eval(args_1_2.data());
                if(m_iso>0.2) m_idiso = fns_["m_iso_binned_ratio"]->eval(args_2_2.data())*fns_["m_id_ratio"]->eval(args_1_2.data());
             } else {
                double mu_iso = 1.0;
                m_idiso = fns_["m_id_ratio"]->eval(args_1_2.data());
                if (m_iso<0.2) {
                  mu_iso = fns_["m_looseiso_embed_ratio"]->eval(args_1_2.data());
                }
                else {
                  mu_iso = fns_["m_iso_binned_ratio"]->eval(args_2_2.data());
                }
                m_idiso*=mu_iso;
             } 
             m_idiso_up = 1.0;
             m_idiso_down = 1.0;
           } else {
             std::vector<Candidate *> muon_vec;
             muon_vec.push_back(dilepton[0]->GetCandidate("lepton2"));
             std::vector<std::pair<Candidate*, PFJet*> > muon_match = MatchByDR(muon_vec,uncleaned_jets,0.2,true,true);
             double jpt_m = 1.0;
             if(muon_match.size()>0) jpt_m = muon_match.at(0).second->pt();
             else jpt_m = m_pt;
             m_idiso = 0.0025*jpt_m+0.902;
             m_idiso_up = (0.0050*jpt_m+0.992)/m_idiso;
             m_idiso_down = 0.812/m_idiso;
          }
          if(gm1_<5 || !do_jlepton_fake_){
           auto args_1_1 = std::vector<double>{e_pt,e_signed_eta};
           auto args_2_1 = std::vector<double>{e_pt,e_signed_eta,e_iso};
           if(!is_embedded_){ 
             if(e_iso<0.15) e_idiso=fns_["e_idiso0p15_desy_ratio"]->eval(args_1_1.data());  
             else e_idiso = fns_["e_iso_binned_ratio"]->eval(args_2_1.data())*fns_["e_id_ratio"]->eval(args_1_1.data());
           }
           else {
                double e_iso_wt = 1.0;
                e_idiso = fns_["e_id_ratio"]->eval(args_1_1.data());
                if(e_iso<0.15) {
                  e_iso_wt = fns_["e_looseiso_ratio"]->eval(args_1_1.data());
                }
                else {
                  e_iso_wt = fns_["e_iso_binned_ratio"]->eval(args_2_1.data());
                }
                e_idiso*=e_iso_wt;
           } 
           e_idiso_down=1.0;
           e_idiso_up=1.0;
          } else {
           std::vector<Candidate *> elec_vec;
           elec_vec.push_back(dilepton[0]->GetCandidate("lepton1"));
           std::vector<std::pair<Candidate*, PFJet*> > elec_match = MatchByDR(elec_vec,uncleaned_jets,0.2,true,true);
           double jpt_e = 1.0;
           if(elec_match.size()>0) jpt_e = elec_match.at(0).second->pt();
           else jpt_e = e_pt;
           e_idiso = 0.0015*jpt_e+0.794;
           e_idiso_up = (0.0030*jpt_e+0.883)/e_idiso;
           e_idiso_down = 0.702/e_idiso;
          }
        } else if(mc_==mc::mc2017 || mc_ == mc::mc2018 || mc_ == mc::mcleg2016) {
          double m_iso = PF04IsolationVal(muon, 0.5, 0); 
          double e_iso = PF03EAIsolationVal(elec, eventInfo->jet_rho()); //lepton_rho
          double e_sceta = elec->sc_eta(); 
          auto args_1 = std::vector<double>{e_pt,e_sceta,e_iso};  
          auto args_2_1 = std::vector<double>{m_pt,m_signed_eta};
          auto args_2_2 = std::vector<double>{m_pt,m_signed_eta,m_iso};  
          if(!is_embedded_){
            e_idiso= fns_["e_idiso_ratio"]->eval(args_1.data());
          }
          if(is_embedded_){
            e_idiso = fns_["e_idiso_embed_ratio"]->eval(args_1.data());
          } 
          if(!is_embedded_) m_idiso = fns_["m_looseiso_ratio"]->eval(args_2_2.data())*fns_["m_id_ratio"]->eval(args_2_1.data());;
          if(is_embedded_) m_idiso = fns_["m_looseiso_embed_ratio"]->eval(args_2_2.data())*fns_["m_id_embed_ratio"]->eval(args_2_1.data());
        }

       weight *= (e_idiso * m_idiso); 
       if (mc_==mc::mc2017 || mc_ == mc::mc2018 || mc_ == mc::mcleg2016) {
         event->Add("idisoweight_1", e_idiso);
         event->Add("idisoweight_2", m_idiso);  
       } else if(mc_!=mc::fall15_76X && mc_!=mc::spring15_74X && mc_!=mc::spring16_80X && mc_ != mc::summer16_80X){
         event->Add("idweight_1", e_idiso);
         event->Add("idweight_2", m_idiso);
       } else { 
         event->Add("idisoweight_1", e_idiso);
         event->Add("idisoweight_2", m_idiso);
         event->Add("idisoweight_up_1", e_idiso_up);
         event->Add("idisoweight_up_2", m_idiso_up);
         event->Add("idisoweight_down_1", e_idiso_down);
         event->Add("idisoweight_down_2", m_idiso_down);
       }
       event->Add("isoweight_1", double(1.0));
       event->Add("isoweight_2", double(1.0));
     } else if (channel_ == channel::mtmet) {
       Muon const* muon = dynamic_cast<Muon const*>(dilepton[0]->GetCandidate("lepton1"));
       double pt = muon->pt();
       double m_eta = fabs(muon->eta());
       double mu_id = 1.0;
       double mu_iso = 1.0;
       if (pt > 8.0 && pt <= 15.0 && m_eta < 0.8)                    { mu_id = 0.9790; mu_iso = 0.9963; }
       if (pt > 8.0 && pt <= 15.0 && m_eta >= 0.8 && m_eta < 1.2)    { mu_id = 0.9809; mu_iso = 0.9769; }
       if (pt > 8.0 && pt <= 15.0 && m_eta >= 1.2)                   { mu_id = 0.9967; mu_iso = 0.9870; }
       if (pt > 15.0 && m_eta < 0.8)                                 { mu_id = 0.9746; mu_iso = 0.9842; }
       if (pt > 15.0 && m_eta >= 0.8 && m_eta < 1.2)                 { mu_id = 0.9796; mu_iso = 0.9664; }
       if (pt > 15.0 && m_eta >= 1.2)                                { mu_id = 0.9864; mu_iso = 0.9795; }
       if (do_id_weights_) mu_iso = 1.0;
       weight *= (mu_id * mu_iso);
       event->Add("idweight_1", mu_id);
       event->Add("idweight_2", double(1.0));
       event->Add("isoweight_1", mu_iso);
       event->Add("isoweight_2", double(1.0));
     } else if (channel_ == channel::zmm){
       Muon const* muon_1 = dynamic_cast<Muon const*>(dilepton[0]->GetCandidate("lepton1"));
       Muon const* muon_2 = dynamic_cast<Muon const*>(dilepton[0]->GetCandidate("lepton2"));
       double m_1_pt = muon_1->pt();
       double m_1_eta = fabs(muon_1->eta());
       double m_1_signed_eta = muon_1->eta();
       double m_2_pt = muon_2->pt();
       double m_2_eta = fabs(muon_2->eta());
       double m_2_signed_eta = muon_2->eta();
       double m_1_iso = PF04IsolationVal(muon_1,0.5,0);
       double m_2_iso = PF04IsolationVal(muon_2,0.5,0);
       double m_1_idiso = 1.0;
       double m_2_idiso = 1.0;
       double m_1_idiso_mc = 1.0;
       double m_1_idiso_data = 1.0;
       double m_2_idiso_mc = 1.0;
       double m_2_idiso_data = 1.0;
       if (mc_ == mc::spring15_74X || mc_==mc::fall15_76X){
         if(m_1_pt<100){
           m_1_idiso_data = em_m_idiso_data_->GetBinContent(em_m_idiso_data_->GetXaxis()->FindBin(m_1_eta),em_m_idiso_data_->GetYaxis()->FindBin(m_1_pt));
           m_1_idiso_mc = em_m_idiso_mc_->GetBinContent(em_m_idiso_mc_->GetXaxis()->FindBin(m_1_eta),em_m_idiso_mc_->GetYaxis()->FindBin(m_1_pt));
         } else {
           m_1_idiso_data = em_m_idiso_data_->GetBinContent(em_m_idiso_data_->GetXaxis()->FindBin(m_1_eta),(em_m_idiso_data_->GetYaxis()->FindBin(m_1_pt)-1));
           m_1_idiso_mc = em_m_idiso_mc_->GetBinContent(em_m_idiso_mc_->GetXaxis()->FindBin(m_1_eta),(em_m_idiso_mc_->GetYaxis()->FindBin(m_1_pt)-1));
         }         
         if(m_2_pt<100){
           m_2_idiso_data = em_m_idiso_data_->GetBinContent(em_m_idiso_data_->GetXaxis()->FindBin(m_2_eta),em_m_idiso_data_->GetYaxis()->FindBin(m_2_pt));
           m_2_idiso_mc = em_m_idiso_mc_->GetBinContent(em_m_idiso_mc_->GetXaxis()->FindBin(m_2_eta),em_m_idiso_mc_->GetYaxis()->FindBin(m_2_pt));
         } else {
           m_2_idiso_data = em_m_idiso_data_->GetBinContent(em_m_idiso_data_->GetXaxis()->FindBin(m_2_eta),(em_m_idiso_data_->GetYaxis()->FindBin(m_2_pt)-1));
           m_2_idiso_mc = em_m_idiso_mc_->GetBinContent(em_m_idiso_mc_->GetXaxis()->FindBin(m_2_eta),(em_m_idiso_mc_->GetYaxis()->FindBin(m_2_pt)-1));
         }         

           m_1_idiso = m_1_idiso_data/m_1_idiso_mc;
           m_2_idiso = m_2_idiso_data/m_2_idiso_mc;
       } else if (mc_ == mc::spring16_80X || mc_ == mc::summer16_80X){
          auto args1_1 = std::vector<double>{m_1_pt,m_1_signed_eta};
          auto args1_2 = std::vector<double>{m_1_pt,m_1_signed_eta,m_1_iso};
          auto args2_1 = std::vector<double>{m_2_pt,m_2_signed_eta};
          auto args2_2 = std::vector<double>{m_2_pt,m_2_signed_eta,m_2_iso};
          
          if(strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16 || strategy_ == strategy::cpdecays16){
            if(is_embedded_){  
              m_1_idiso = fns_["m_id_ratio"]->eval(args1_1.data())*fns_["m_iso_binned_ratio"]->eval(args1_2.data());
              m_2_idiso = fns_["m_id_ratio"]->eval(args2_1.data())*fns_["m_iso_binned_ratio"]->eval(args2_2.data());
            } else {
              m_1_idiso = fns_["m_idiso0p15_desy_ratio"]->eval(args1_1.data());
              m_2_idiso = fns_["m_idiso0p15_desy_ratio"]->eval(args2_1.data());  
              if(m_1_iso>0.15) m_1_idiso = fns_["m_idiso_aiso0p15to0p3_desy_ratio"]->eval(args1_1.data());
              if(m_2_iso>0.15) m_2_idiso = fns_["m_idiso_aiso0p15to0p3_desy_ratio"]->eval(args2_1.data()); 
            }
          } else {
            m_1_idiso = fns_["m_id_ratio"]->eval(args1_1.data()) * fns_["m_iso_binned_ratio"]->eval(args1_2.data()) ;
            m_2_idiso = fns_["m_id_ratio"]->eval(args2_1.data()) * fns_["m_iso_binned_ratio"]->eval(args2_2.data()) ;
          }
          
          
         /*if(m_1_pt<1000){
           m_1_idiso_data = em_m_idiso_data_->GetBinContent(em_m_idiso_data_->GetXaxis()->FindBin(m_1_eta),em_m_idiso_data_->GetYaxis()->FindBin(m_1_pt));
           m_1_idiso_mc = em_m_idiso_mc_->GetBinContent(em_m_idiso_mc_->GetXaxis()->FindBin(m_1_eta),em_m_idiso_mc_->GetYaxis()->FindBin(m_1_pt));
         } else {
           m_1_idiso_data = em_m_idiso_data_->GetBinContent(em_m_idiso_data_->GetXaxis()->FindBin(m_1_eta),(em_m_idiso_data_->GetYaxis()->FindBin(m_1_pt)-1));
           m_1_idiso_mc = em_m_idiso_mc_->GetBinContent(em_m_idiso_mc_->GetXaxis()->FindBin(m_1_eta),(em_m_idiso_mc_->GetYaxis()->FindBin(m_1_pt)-1));
         }         
         if(m_2_pt<1000){
           m_2_idiso_data = em_m_idiso_data_->GetBinContent(em_m_idiso_data_->GetXaxis()->FindBin(m_2_eta),em_m_idiso_data_->GetYaxis()->FindBin(m_2_pt));
           m_2_idiso_mc = em_m_idiso_mc_->GetBinContent(em_m_idiso_mc_->GetXaxis()->FindBin(m_2_eta),em_m_idiso_mc_->GetYaxis()->FindBin(m_2_pt));
         } else {
           m_2_idiso_data = em_m_idiso_data_->GetBinContent(em_m_idiso_data_->GetXaxis()->FindBin(m_2_eta),(em_m_idiso_data_->GetYaxis()->FindBin(m_2_pt)-1));
           m_2_idiso_mc = em_m_idiso_mc_->GetBinContent(em_m_idiso_mc_->GetXaxis()->FindBin(m_2_eta),(em_m_idiso_mc_->GetYaxis()->FindBin(m_2_pt)-1));
         }         

           m_1_idiso = m_1_idiso_data/m_1_idiso_mc;
           m_2_idiso = m_2_idiso_data/m_2_idiso_mc;*/
       } else if (mc_==mc::mc2017 || mc_ == mc::mc2018 || mc_ == mc::mcleg2016){
         auto args1_2 = std::vector<double>{m_1_pt,m_1_signed_eta,m_1_iso};
         auto args2_2 = std::vector<double>{m_2_pt,m_2_signed_eta,m_2_iso};
         double looseiso_wt = 1.; 
         if(is_embedded_){
           m_1_idiso = fns_["m_idiso_embed_ratio"]->eval(args1_2.data());
           m_2_idiso = fns_["m_idiso_embed_ratio"]->eval(args2_2.data());
           looseiso_wt = fns_["m_looseiso_embed_ratio"]->eval(args2_2.data())*fns_["m_looseiso_embed_ratio"]->eval(args1_2.data()) * fns_["m_id_embed_ratio"]->eval(args2_2.data())*fns_["m_id_embed_ratio"]->eval(args1_2.data())/(m_1_idiso*m_2_idiso);
         } else {
           m_1_idiso = fns_["m_idiso_ratio"]->eval(args1_2.data());
           m_2_idiso = fns_["m_idiso_ratio"]->eval(args2_2.data());
           looseiso_wt = fns_["m_looseiso_ratio"]->eval(args2_2.data())*fns_["m_looseiso_ratio"]->eval(args1_2.data()) * fns_["m_id_ratio"]->eval(args2_2.data())*fns_["m_id_ratio"]->eval(args1_2.data())/(m_1_idiso*m_2_idiso);
         }
         event->Add("looseiso_wt", looseiso_wt);
       }

       weight *= (m_1_idiso * m_2_idiso);
       event->Add("idisoweight_1", m_1_idiso);
       event->Add("idisoweight_2", m_2_idiso);
       event->Add("isoweight_1", double(1.0));
       event->Add("isoweight_2", double(1.0));
     } else if (channel_ == channel::zee){
         Electron const* ele_1 = dynamic_cast<Electron const*>(dilepton[0]->GetCandidate("lepton1"));
         Electron const* ele_2 = dynamic_cast<Electron const*>(dilepton[0]->GetCandidate("lepton2"));
         double e_1_pt = ele_1->pt();
         double e_1_eta = fabs(ele_1->eta());
         double e_2_pt = ele_2->pt();
         double e_2_eta = fabs(ele_2->eta());
         double e_1_idiso = 1.0;
         double e_2_idiso = 1.0;
         double e_1_idiso_mc = 1.0;
         double e_1_idiso_data = 1.0;
         double e_2_idiso_mc = 1.0;
         double e_2_idiso_data = 1.0;
         double e_2_signed_eta = ele_2->eta();
         double e_1_signed_eta = ele_1->eta();
         double e_1_iso = PF03IsolationVal(ele_1,0.5,0);
         double e_2_iso = PF03IsolationVal(ele_2,0.5,0);
         if (mc_ == mc::spring15_74X || mc_==mc::fall15_76X){

           if(e_1_pt<100){
             e_1_idiso_data = em_e_idiso_data_->GetBinContent(em_e_idiso_data_->GetXaxis()->FindBin(e_1_eta),em_e_idiso_data_->GetYaxis()->FindBin(e_1_pt));
             e_1_idiso_mc = em_e_idiso_mc_->GetBinContent(em_e_idiso_mc_->GetXaxis()->FindBin(e_1_eta),em_e_idiso_mc_->GetYaxis()->FindBin(e_1_pt));
           } else {
             e_1_idiso_data = em_e_idiso_data_->GetBinContent(em_e_idiso_data_->GetXaxis()->FindBin(e_1_eta),(em_e_idiso_data_->GetYaxis()->FindBin(e_1_pt)-1));
             e_1_idiso_mc = em_e_idiso_mc_->GetBinContent(em_e_idiso_mc_->GetXaxis()->FindBin(e_1_eta),(em_e_idiso_mc_->GetYaxis()->FindBin(e_1_pt)-1));
           }         
           if(e_2_pt<100){
             e_2_idiso_data = em_e_idiso_data_->GetBinContent(em_e_idiso_data_->GetXaxis()->FindBin(e_2_eta),em_e_idiso_data_->GetYaxis()->FindBin(e_2_pt));
             e_2_idiso_mc = em_e_idiso_mc_->GetBinContent(em_e_idiso_mc_->GetXaxis()->FindBin(e_2_eta),em_e_idiso_mc_->GetYaxis()->FindBin(e_2_pt));
           } else {
             e_2_idiso_data = em_e_idiso_data_->GetBinContent(em_e_idiso_data_->GetXaxis()->FindBin(e_2_eta),(em_e_idiso_data_->GetYaxis()->FindBin(e_2_pt)-1));
             e_2_idiso_mc = em_e_idiso_mc_->GetBinContent(em_e_idiso_mc_->GetXaxis()->FindBin(e_2_eta),(em_e_idiso_mc_->GetYaxis()->FindBin(e_2_pt)-1));
           }         

             e_1_idiso = e_1_idiso_data/e_1_idiso_mc;
             e_2_idiso = e_2_idiso_data/e_2_idiso_mc;
         } else if (mc_ == mc::spring16_80X || mc_ == mc::summer16_80X){
            auto args1_1 = std::vector<double>{e_1_pt,e_1_signed_eta};
            auto args1_2 = std::vector<double>{e_1_pt,e_1_signed_eta,e_1_iso};
            auto args2_1 = std::vector<double>{e_2_pt,e_2_signed_eta};
            auto args2_2 = std::vector<double>{e_2_pt,e_2_signed_eta,e_2_iso};
            if(strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16 || strategy_ == strategy::cpdecays16){
            e_1_idiso = fns_["e_idiso0p1_desy_ratio"]->eval(args1_1.data()); 
            e_2_idiso = fns_["e_idiso0p1_desy_ratio"]->eval(args2_1.data()); 
            if(e_1_iso>0.1) e_1_idiso = fns_["e_idiso_aiso0p1to0p3_desy_ratio"]->eval(args1_1.data());
            if(e_2_iso>0.1) e_2_idiso = fns_["e_idiso_aiso0p1to0p3_desy_ratio"]->eval(args2_1.data());
            } else {
              e_1_idiso = fns_["e_id_ratio"]->eval(args1_1.data()) * fns_["e_iso_binned_ratio"]->eval(args1_2.data()) ;
              e_2_idiso = fns_["e_id_ratio"]->eval(args2_1.data()) * fns_["e_iso_binned_ratio"]->eval(args2_2.data()) ;
            }
         } else if (mc_==mc::mc2017 || mc_ == mc::mc2018 || mc_ == mc::mcleg2016){
           e_1_iso = PF03EAIsolationVal(ele_1, eventInfo->jet_rho());
           e_2_iso = PF03EAIsolationVal(ele_2, eventInfo->jet_rho());
           double e_1_sceta = ele_1->sc_eta();   
           double e_2_sceta = ele_2->sc_eta();
           auto args1_1 = std::vector<double>{e_1_pt,e_1_sceta,e_1_iso};
           auto args2_1 = std::vector<double>{e_2_pt,e_2_sceta,e_2_iso};

           if(is_embedded_){
             e_1_idiso = fns_["e_idiso_embed_ratio"]->eval(args1_1.data()); 
             e_2_idiso = fns_["e_idiso_embed_ratio"]->eval(args2_1.data()); 
           } else {
             e_1_idiso = fns_["e_idiso_ratio"]->eval(args1_1.data()); 
             e_2_idiso = fns_["e_idiso_ratio"]->eval(args2_1.data());
           }
         } 

         weight *= (e_1_idiso * e_2_idiso);
         event->Add("idisoweight_1", e_1_idiso);
         event->Add("idisoweight_2", e_2_idiso);
         event->Add("isoweight_1", double(1.0));
         event->Add("isoweight_2", double(1.0));
        }
   }
   eventInfo->set_weight("lepton", weight);


   if (do_tt_muon_weights_) {
     Muon const* muon = dynamic_cast<Muon const*>(dilepton[0]->GetCandidate("lepton2"));
     double m_pt = muon->pt();
     double m_eta = fabs(muon->eta());
     double m_wt = 1.0;
     /*
     if (m_eta < 0.4) {
       if (m_pt > 20.0 && m_pt <= 40.0)  m_wt = 1.005;
       if (m_pt > 40.0 && m_pt <= 60.0)  m_wt = 0.982;
       if (m_pt > 60.0 && m_pt <= 80.0)  m_wt = 0.933;
       if (m_pt > 80.0 && m_pt <= 100.0) m_wt = 0.893;
       if (m_pt > 100.0)                 m_wt = 0.939;
     } else if (m_eta >= 0.4 && m_eta < 1.2) {
       if (m_pt > 20.0 && m_pt <= 40.0)  m_wt = 1.009;
       if (m_pt > 40.0 && m_pt <= 60.0)  m_wt = 1.061;
       if (m_pt > 60.0 && m_pt <= 80.0)  m_wt = 1.054;
       if (m_pt > 80.0 && m_pt <= 100.0) m_wt = 1.144;
       if (m_pt > 100.0)                 m_wt = 1.206;
     } else if (m_eta >= 1.2 && m_eta < 1.6) {
       if (m_pt > 20.0 && m_pt <= 40.0)  m_wt = 0.987;
       if (m_pt > 40.0 && m_pt <= 60.0)  m_wt = 1.074;
       if (m_pt > 60.0 && m_pt <= 80.0)  m_wt = 1.073;
       if (m_pt > 80.0 && m_pt <= 100.0) m_wt = 1.149;
       if (m_pt > 100.0)                 m_wt = 1.443;
     } else {
       if (m_pt > 20.0 && m_pt <= 40.0)  m_wt = 1.075;
       if (m_pt > 40.0 && m_pt <= 60.0)  m_wt = 1.090;
       if (m_pt > 60.0 && m_pt <= 80.0)  m_wt = 1.172;
       if (m_pt > 80.0 && m_pt <= 100.0) m_wt = 1.297;
       if (m_pt > 100.0)                 m_wt = 1.603;
     }
     */
     if (m_eta < 0.4) {
       if (m_pt > 20.0 && m_pt <= 30.0)  m_wt = 1.027;
       if (m_pt > 30.0 && m_pt <= 50.0)  m_wt = 0.932;
       if (m_pt > 50.0 && m_pt <= 75.0)  m_wt = 0.889;
       if (m_pt > 75.0 && m_pt <= 100.0) m_wt = 0.913;
       if (m_pt > 100.0)                 m_wt = 0.913;
     } else if (m_eta >= 0.4 && m_eta < 0.8) {
       if (m_pt > 20.0 && m_pt <= 30.0)  m_wt = 1.068;
       if (m_pt > 30.0 && m_pt <= 50.0)  m_wt = 0.950;
       if (m_pt > 50.0 && m_pt <= 75.0)  m_wt = 1.060;
       if (m_pt > 75.0 && m_pt <= 100.0) m_wt = 1.009;
       if (m_pt > 100.0)                 m_wt = 1.009;
     } else if (m_eta >= 0.8) {
       if (m_pt > 20.0 && m_pt <= 30.0)  m_wt = 0.954;
       if (m_pt > 30.0 && m_pt <= 50.0)  m_wt = 1.068;
       if (m_pt > 50.0 && m_pt <= 75.0)  m_wt = 1.060;
       if (m_pt > 75.0 && m_pt <= 100.0) m_wt = 1.056;
       if (m_pt > 100.0)                 m_wt = 1.056;
     }
     eventInfo->set_weight("tt_muon_weight", m_wt);
   }


   if (do_emu_e_fakerates_) {
     Electron const* elec = dynamic_cast<Electron const*>(dilepton[0]->GetCandidate("lepton1"));
     double elefopt = (elec->pt() < 35) ? elec->pt() : 34.99;
     double eleEta = elec->eta();
     if (era_ == era::data_2012_rereco) eleEta = fabs(eleEta);
     int eleptbin = ElectronFakeRateHist_PtEta->GetXaxis()->FindFixBin(elefopt);
     int eleetabin = ElectronFakeRateHist_PtEta->GetYaxis()->FindFixBin(eleEta);    
     double eleprob = ElectronFakeRateHist_PtEta->GetBinContent(eleptbin,eleetabin);
     double elefakerate = eleprob/(1.0 - eleprob);
     //double elefakerate_errlow = ElectronFakeRateHist_PtEta->GetError(elefopt,fabs(elec->eta()),mithep::TH2DAsymErr::kStatErrLow)/pow((1-ElectronFakeRateHist_PtEta->GetError(elefopt,fabs(elec->eta()),mithep::TH2DAsymErr::kStatErrLow)),2);
     //double elefakerate_errhigh = ElectronFakeRateHist_PtEta->GetError(elefopt,fabs(elec->eta()),mithep::TH2DAsymErr::kStatErrHigh)/pow((1-ElectronFakeRateHist_PtEta->GetError(elefopt,fabs(elec->eta()),mithep::TH2DAsymErr::kStatErrHigh)),2);
     eventInfo->set_weight("emu_e_fakerate", elefakerate);
   }

   if (do_emu_m_fakerates_) {
     Muon const* muon = dynamic_cast<Muon const*>(dilepton[0]->GetCandidate("lepton2"));
     Double_t mufopt = (muon->pt() < 35) ? muon->pt() : 34.99;
     double muEta = muon->eta();
     if (era_ == era::data_2012_rereco) muEta = fabs(muEta);
     int muptbin = MuonFakeRateHist_PtEta->GetXaxis()->FindFixBin(mufopt);
     int muetabin = MuonFakeRateHist_PtEta->GetYaxis()->FindFixBin(muEta);    
     double muprob = MuonFakeRateHist_PtEta->GetBinContent(muptbin,muetabin);
     double mufakerate = muprob/(1.0 - muprob);
     //double mufakerate_errlow = MuonFakeRateHist_PtEta->GetError(mufopt,fabs(muon->eta()),mithep::TH2DAsymErr::kStatErrLow)/pow((1-MuonFakeRateHist_PtEta->GetError(mufopt,fabs(muon->eta()),mithep::TH2DAsymErr::kStatErrLow)),2);
     //double mufakerate_errhigh = MuonFakeRateHist_PtEta->GetError(mufopt,fabs(muon->eta()),mithep::TH2DAsymErr::kStatErrHigh)/pow((1-MuonFakeRateHist_PtEta->GetError(mufopt,fabs(muon->eta()),mithep::TH2DAsymErr::kStatErrHigh)),2);
     eventInfo->set_weight("emu_m_fakerate", mufakerate);
   }

   if (do_etau_fakerate_ && era_!=era::data_2015 && era_!=era::data_2016 && era_ != era::data_2017 && era_ != era::data_2018) {
     std::vector<GenParticle *> parts = event->GetPtrVec<GenParticle>("genParticles");
     ic::erase_if(parts, !(boost::bind(&GenParticle::status, _1) == 3));
     ic::erase_if(parts, ! ((boost::bind(&GenParticle::pdgid, _1) == 11)||(boost::bind(&GenParticle::pdgid, _1) == -11)) );
     ic::erase_if(parts, ! (boost::bind(MinPtMaxEta, _1, 8.0, 2.6)));
     std::vector<Candidate *> tau_cand;
     tau_cand.push_back(dilepton[0]->GetCandidate("lepton2"));
     Tau const* tau = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton2"));
     std::vector<std::pair<Candidate*, GenParticle*> > matches = MatchByDR(tau_cand, parts, 0.5, true, true);
     // Mode 0 Barrel: 0.87 +/- 0.04
     // Mode 0 Endcap: 0.40 +/- 0.20
     // Mode 1 Barrel: 1.19 +/- 0.06
     // Mode 1 Endcap: 0.72 +/ 0.17
     if (matches.size() > 0) {
       if (mc_ == mc::fall11_42X) {
         if (fabs(tau_cand[0]->eta()) < 1.5) {
           if (tau->decay_mode() == 0) eventInfo->set_weight("etau_fakerate", 1.142);
           if (tau->decay_mode() == 1) eventInfo->set_weight("etau_fakerate", 1.617);
         } else {
           if (tau->decay_mode() == 0) eventInfo->set_weight("etau_fakerate", 0.859);
           if (tau->decay_mode() == 1) eventInfo->set_weight("etau_fakerate", 0.610);
         }
       } else {
         if (era_ == era::data_2012_rereco) {
           if (fabs(tau_cand[0]->eta()) < 1.5) {
             if (tau->decay_mode() == 0) eventInfo->set_weight("etau_fakerate", 1.37);
             if (tau->decay_mode() == 1) eventInfo->set_weight("etau_fakerate", 2.18);
           } else {
             if (tau->decay_mode() == 0) eventInfo->set_weight("etau_fakerate", 1.11);
             if (tau->decay_mode() == 1) eventInfo->set_weight("etau_fakerate", 0.47);
           }
         }
       }
     }
   }

   if(do_etau_fakerate_ && (era_==era::data_2016 || era_==era::data_2017 || era_ == era::data_2018) && (mc_==mc::mc2017 || mc_ == mc::mc2018 || mc_ == mc::mcleg2016) ) {
     double etau_fakerate_1=1.0;
     double etau_fakerate_2=1.0;
     double etau_fakerate_1_up=1.0;
     double etau_fakerate_1_down=1.0;
     double etau_fakerate_2_up=1.0;
     double etau_fakerate_2_down=1.0;

     Tau const* tau = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton2"));
     double t_eta = fabs(tau->eta());
     auto t_args = std::vector<double>{t_eta};
     unsigned gm1_ = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_1"));
     unsigned gm2_ = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_2"));
     if(gm2_==1||gm2_==3){
       if(channel_ == channel::et) {
         etau_fakerate_2 = fns_["t_id_vs_e_eta_tight"]->eval(t_args.data());
         etau_fakerate_2_up = fns_["t_id_vs_e_eta_tight_up"]->eval(t_args.data());
         etau_fakerate_2_down = fns_["t_id_vs_e_eta_tight_down"]->eval(t_args.data());
       }
       else {
         etau_fakerate_2 = fns_["t_id_vs_e_eta_vvloose"]->eval(t_args.data());
         etau_fakerate_2_up = fns_["t_id_vs_e_eta_vvloose_up"]->eval(t_args.data());
         etau_fakerate_2_down = fns_["t_id_vs_e_eta_vvloose_down"]->eval(t_args.data());
       }
     }
     if(channel_ == channel::tt && (gm1_==1||gm1_==3)) {
       Tau const* tau1 = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton1"));
       double t_1_eta = fabs(tau1->eta());
       auto t_1_args = std::vector<double>{t_1_eta};
       etau_fakerate_1 = fns_["t_id_vs_e_eta_vvloose"]->eval(t_1_args.data());
       etau_fakerate_1_up = fns_["t_id_vs_e_eta_vvloose_up"]->eval(t_1_args.data());
       etau_fakerate_1_down = fns_["t_id_vs_e_eta_vvloose_down"]->eval(t_1_args.data());
     }

     eventInfo->set_weight("etau_fakerate",etau_fakerate_1*etau_fakerate_2);
     event->Add("wt_efake_rate_up",etau_fakerate_1_up*etau_fakerate_2_up/(etau_fakerate_1*etau_fakerate_2));
     event->Add("wt_efake_rate_down",etau_fakerate_1_down*etau_fakerate_2_down/(etau_fakerate_1*etau_fakerate_2));

   }


   if (do_etau_fakerate_ && (era_==era::data_2015||era_==era::data_2016 || era_==era::data_2017 || era_ == era::data_2018) && !(mc_==mc::mc2017 || mc_ == mc::mc2018 || mc_ == mc::mcleg2016)) {
     unsigned gm2_ = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_2"));
     Tau const* tau = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton2"));
     double etau_fakerate_1=1.0;
     double etau_fakerate_2=1.0;
     if(mc_ == mc::summer16_80X || mc_ == mc::mcleg2016){
       if(channel_ == channel::et){
         if (tau->GetTauID("againstElectronTightMVA6")<0.5) etau_fakerate_2=1.0; 
         else if(gm2_==1||gm2_==3){
           if(fabs(tau->eta()) < 1.5){
              etau_fakerate_2 = 1.40;
           } else etau_fakerate_2=1.90;
           if(strategy_==strategy::smsummer16 || strategy_ == strategy::cpsummer16 ||  strategy_ == strategy::legacy16 || strategy_ == strategy::cpdecays16){
             if(fabs(tau->eta()) < 1.460){
              etau_fakerate_2 = 1.40;
             } else if(fabs(tau->eta()) > 1.558)  etau_fakerate_2=1.9;
             event->Add("wt_lfake_rate_up",1.12);
             event->Add("wt_lfake_rate_down",0.88);
             double wt_lfake_rate = 1.0;
             double dm_2_ = tau->decay_mode();
             if(dm_2_==0) wt_lfake_rate = 0.98;
             if(dm_2_==1) wt_lfake_rate = 1.2;
             event->Add("wt_lfake_rate", wt_lfake_rate);
             //store m_vis shift to apply in HTTCategories
             double m_vis_shift = 1.;
             if(dm_2_==0) m_vis_shift = 1.017;
             if(dm_2_==1) m_vis_shift = 1.03;
             event->Add("m_vis_shift", m_vis_shift);
                                                       
           }
         }
     } else {
         if (tau->GetTauID("againstElectronVLooseMVA6")<0.5) etau_fakerate_2=1.0; 
         else if(gm2_==1||gm2_==3){
           if(fabs(tau->eta()) < 1.5){
              etau_fakerate_2=1.21;
           } else etau_fakerate_2=1.38;
           if(strategy_==strategy::smsummer16 || strategy_ == strategy::cpsummer16 ||  strategy_ == strategy::legacy16 || strategy_ == strategy::cpdecays16){
             if(fabs(tau->eta()) < 1.460){
              etau_fakerate_2 = 1.21;
             } else if(fabs(tau->eta()) > 1.558)  etau_fakerate_2=1.38;
           }
         }
       }
       if(channel_ == channel::tt){
         unsigned gm1_ = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_1"));
         Tau const* tau1 = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton1"));
         if (tau1->GetTauID("againstElectronVLooseMVA6")<0.5) etau_fakerate_1=1.0;
         else if(gm1_==1||gm1_==3){
           if(fabs(tau1->eta()) < 1.5){
              etau_fakerate_1=1.21;
           } else etau_fakerate_1=1.38;
           if(strategy_==strategy::smsummer16 || strategy_ == strategy::cpsummer16 ||  strategy_ == strategy::legacy16 || strategy_ == strategy::cpdecays16){
             if(fabs(tau1->eta()) < 1.460){
               etau_fakerate_1 = 1.21;
              } else if(fabs(tau1->eta()) > 1.558)  etau_fakerate_1=1.38;   
           }
         }
       }  
     } else if(mc_ == mc::mc2017){
       if(channel_ == channel::et){
         if (tau->GetTauID("againstElectronTightMVA6")<0.5) etau_fakerate_2 = 1.0;
         else if(gm2_==1||gm2_==3){
           if(fabs(tau->eta()) < 1.460){
            etau_fakerate_2 = 1.80;
           } else if(fabs(tau->eta()) > 1.558)  etau_fakerate_2=1.53;
           if(tau->decay_mode()<2) etau_fakerate_2*=0.81; 
         }
     } else {
         if (tau->GetTauID("againstElectronVLooseMVA6")<0.5) etau_fakerate_2 = 1.0;
         else if(gm2_==1||gm2_==3){
           if(fabs(tau->eta()) < 1.460){
             etau_fakerate_2 = 1.09;
           } else if(fabs(tau->eta()) > 1.558)  etau_fakerate_2=1.19;
         }
       }
       if(channel_ == channel::tt){
         unsigned gm1_ = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_1"));
         Tau const* tau1 = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton1"));
         if (tau1->GetTauID("againstElectronVLooseMVA6")<0.5) etau_fakerate_1 = 1.0;
         else if(gm1_==1||gm1_==3){
           if(strategy_==strategy::smsummer16 || strategy_ == strategy::cpsummer16 ||  strategy_ == strategy::legacy16 || strategy_ == strategy::cpdecays16){
             if(fabs(tau1->eta()) < 1.460){
               etau_fakerate_1 = 1.09;
              } else if(fabs(tau1->eta()) > 1.558)  etau_fakerate_1=1.19;   
           }
         }
       }  
     } else if(mc_ == mc::mc2018){
       if(channel_ == channel::et){
         if (tau->GetTauID("againstElectronTightMVA6")<0.5) etau_fakerate_2 = 1.0;
         else if(gm2_==1||gm2_==3){
           if(fabs(tau->eta()) < 1.448){
            etau_fakerate_2 = 1.78;
           } else if(fabs(tau->eta()) > 1.558)  etau_fakerate_2=1.55;
           if(tau->decay_mode()<2) etau_fakerate_2*=0.81; 
         }
     } else {
         if (tau->GetTauID("againstElectronVLooseMVA6")<0.5) etau_fakerate_2 = 1.0;
         else if(gm2_==1||gm2_==3){
           if(fabs(tau->eta()) < 1.448){
             etau_fakerate_2 = 1.089;
           } else if(fabs(tau->eta()) > 1.558)  etau_fakerate_2=1.189;
         }
       }
       if(channel_ == channel::tt){
         unsigned gm1_ = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_1"));
         Tau const* tau1 = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton1"));
         if (tau1->GetTauID("againstElectronVLooseMVA6")<0.5) etau_fakerate_1 = 1.0;
         else if(gm1_==1||gm1_==3){
           if(fabs(tau1->eta()) < 1.448){
             etau_fakerate_1 = 1.089;
           } else if(fabs(tau1->eta()) > 1.558)  etau_fakerate_1=1.189;   
         }
       }  
     } else {
       if(channel_ == channel::et){
         if(gm2_==1||gm2_==3){
           if(fabs(tau->eta()) < 1.5){
              etau_fakerate_2 = 1.80;
           } else etau_fakerate_2=1.30;
         }
       } else {
         if(gm2_==1||gm2_==3){
           if(fabs(tau->eta()) < 1.5){
              etau_fakerate_2=1.02;
           } else etau_fakerate_2=1.11;
         }
       }
       if(channel_ == channel::tt){
         unsigned gm1_ = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_1"));
         Tau const* tau1 = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton1"));
         if(gm1_==1||gm1_==3){
           if(fabs(tau1->eta()) < 1.5){
              etau_fakerate_1=1.02;
           } else etau_fakerate_1=1.11;
         }
       }
     }
    eventInfo->set_weight("etau_fakerate",etau_fakerate_1*etau_fakerate_2);
   }

   if (do_mtau_fakerate_ && era_!=era::data_2016 && era_ != era::data_2017 && era_ != era::data_2018) {
     std::vector<GenParticle *> parts = event->GetPtrVec<GenParticle>("genParticles");
     ic::erase_if(parts, !(boost::bind(&GenParticle::status, _1) == 3));
     ic::erase_if(parts, ! ((boost::bind(&GenParticle::pdgid, _1) == 13)||(boost::bind(&GenParticle::pdgid, _1) == -13)) );
     ic::erase_if(parts, ! (boost::bind(MinPtMaxEta, _1, 8.0, 2.6)));
     std::vector<Candidate *> tau_cand;
     tau_cand.push_back(dilepton[0]->GetCandidate("lepton2"));
     std::vector<std::pair<Candidate*, GenParticle*> > matches = MatchByDR(tau_cand, parts, 0.5, true, true);
     //We didnt use this for any of 2011 or 2012 in the end, just leaving the code here with a weight of 1 for now.
     if (matches.size() > 0) {
      eventInfo->set_weight("mtau_fakerate", 1.00);
     }
   }
  
   if(do_mtau_fakerate_ && (era_==era::data_2016 || era_==era::data_2017 || era_ == era::data_2018) && (mc_==mc::mc2017 || mc_ == mc::mc2018 || mc_ == mc::mcleg2016) ) {
     double mtau_fakerate_1=1.0;
     double mtau_fakerate_2=1.0;
     double mtau_fakerate_1_up=1.0;
     double mtau_fakerate_1_down=1.0;
     double mtau_fakerate_2_up=1.0;
     double mtau_fakerate_2_down=1.0;

     Tau const* tau = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton2"));
     double t_eta = fabs(tau->eta());
     auto t_args = std::vector<double>{t_eta};
     unsigned gm1_ = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_1"));
     unsigned gm2_ = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_2"));
     if (gm2_==2||gm2_==4){
       if(channel_ == channel::mt) {
         mtau_fakerate_2 = fns_["t_id_vs_mu_eta_tight"]->eval(t_args.data());
         mtau_fakerate_2_up = fns_["t_id_vs_mu_eta_tight_up"]->eval(t_args.data());
         mtau_fakerate_2_down = fns_["t_id_vs_mu_eta_tight_down"]->eval(t_args.data());
       }
       else {                       
         mtau_fakerate_2 = fns_["t_id_vs_mu_eta_vloose"]->eval(t_args.data()); 
         mtau_fakerate_2_up = fns_["t_id_vs_mu_eta_vloose_up"]->eval(t_args.data());
         mtau_fakerate_2_down = fns_["t_id_vs_mu_eta_vloose_down"]->eval(t_args.data());
       }
     }
     if(channel_ == channel::tt && (gm1_==2||gm1_==4)) {
       Tau const* tau1 = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton1"));
       double t_1_eta = fabs(tau1->eta());
       auto t_1_args = std::vector<double>{t_1_eta};
       mtau_fakerate_1 = fns_["t_id_vs_mu_eta_vloose"]->eval(t_1_args.data()); 
       mtau_fakerate_1_up = fns_["t_id_vs_mu_eta_vloose_up"]->eval(t_1_args.data());
       mtau_fakerate_1_down = fns_["t_id_vs_mu_eta_vloose_down"]->eval(t_1_args.data());
     }

     eventInfo->set_weight("mtau_fakerate",mtau_fakerate_1*mtau_fakerate_2);
     event->Add("wt_mfake_rate_up",mtau_fakerate_1_up*mtau_fakerate_2_up/(mtau_fakerate_1*mtau_fakerate_2));
     event->Add("wt_mfake_rate_down",mtau_fakerate_1_down*mtau_fakerate_2_down/(mtau_fakerate_1*mtau_fakerate_2));

   }
 
   if (do_mtau_fakerate_ && (era_==era::data_2016 || era_==era::data_2017 || era_ == era::data_2018) && !(mc_==mc::mc2017 || mc_ == mc::mc2018 || mc_ == mc::mcleg2016)) {
     unsigned gm2_ = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_2"));
     Tau const* tau = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton2"));
     double mtau_fakerate_1=1.0;
     double mtau_fakerate_2=1.0;
     if(mc_ == mc::summer16_80X || mc_ == mc::mcleg2016){
       if(channel_ == channel::mt){
         if(tau->GetTauID("againstMuonTight3")<0.5) mtau_fakerate_2 = 1.0;
         else if((gm2_==2||gm2_==4)){
           if(fabs(tau->eta()) < 0.4){
             mtau_fakerate_2 = 1.47;
           } else if(fabs(tau->eta()) < 0.8){
             mtau_fakerate_2 = 1.55;
           } else if(fabs(tau->eta()) < 1.2){
             mtau_fakerate_2 = 1.33;
           } else if(fabs(tau->eta()) < 1.7){
             mtau_fakerate_2=1.72;
           } else if(fabs(tau->eta()) < 2.3){
             mtau_fakerate_2=2.5;
           }
           if(false){
             if(fabs(tau->eta()) < 0.4){
               mtau_fakerate_2 = 1.26;
             } else if(fabs(tau->eta()) < 0.8){
               mtau_fakerate_2 = 1.36;
             } else if(fabs(tau->eta()) < 1.2){
               mtau_fakerate_2 = 0.85;
             } else if(fabs(tau->eta()) < 1.7){
               mtau_fakerate_2=1.7;
             } else if(fabs(tau->eta()) < 2.3){
               mtau_fakerate_2=2.3;
             }  
             event->Add("wt_lfake_rate_up",1.25);
             event->Add("wt_lfake_rate_down",0.75);
             //not clear if the scale factors split by dm is applied for the SM or not so add these to the event so they can be applied at plotting level if required
             double wt_lfake_rate = 1.0;
             double dm_2_ = tau->decay_mode();
             if(dm_2_==0) wt_lfake_rate = 0.75;
             if(dm_2_==1) wt_lfake_rate = 1.0;
             event->Add("wt_lfake_rate", wt_lfake_rate);
           }
         }
       } else {
         if(tau->GetTauID("againstMuonLoose3")<0.5) mtau_fakerate_2 = 1.0; 
         else if(gm2_==2||gm2_==4){
           if(fabs(tau->eta()) < 0.4){
             mtau_fakerate_2=1.22;
           } else if(fabs(tau->eta()) < 0.8){
             mtau_fakerate_2=1.12;
           } else if(fabs(tau->eta()) < 1.2){
             mtau_fakerate_2=1.26;
           } else if(fabs(tau->eta()) < 1.7){
             mtau_fakerate_2=1.22;
           } else if(fabs(tau->eta()) < 2.3){
             mtau_fakerate_2=2.39;
           }
           if(false){
            if(fabs(tau->eta()) < 0.4){
              mtau_fakerate_2=1.01;
            } else if(fabs(tau->eta()) < 0.8){
              mtau_fakerate_2=1.01;
            } else if(fabs(tau->eta()) < 1.2){
              mtau_fakerate_2=0.87;
            } else if(fabs(tau->eta()) < 1.7){
              mtau_fakerate_2=1.2;
            } else if(fabs(tau->eta()) < 2.3){
              mtau_fakerate_2=2.3;
            }
          }
         }
       }
       if(channel_ == channel::tt){
       unsigned gm1_ = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_1"));
       Tau const* tau1 = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton1"));
         if(tau1->GetTauID("againstMuonTight3")<0.5) mtau_fakerate_1 = 1.0; 
         else if(gm1_==2||gm1_==4){
           if(fabs(tau1->eta()) < 0.4){
             mtau_fakerate_1=1.22;
           } else if(fabs(tau1->eta()) < 0.8){
             mtau_fakerate_1=1.12;
           } else if(fabs(tau1->eta()) < 1.2){
             mtau_fakerate_1=1.26;
           } else if(fabs(tau1->eta()) < 1.7){
             mtau_fakerate_1=1.22;
           } else if(fabs(tau1->eta()) < 2.3){
             mtau_fakerate_1=2.39;
           }
           if(false){
             if(fabs(tau1->eta()) < 0.4){
               mtau_fakerate_1=1.01;
             } else if(fabs(tau1->eta()) < 0.8){
               mtau_fakerate_1=1.01;
             } else if(fabs(tau1->eta()) < 1.2){
               mtau_fakerate_1=0.87;
             } else if(fabs(tau1->eta()) < 1.7){
               mtau_fakerate_1=1.2;
             } else if(fabs(tau1->eta()) < 2.3){
               mtau_fakerate_1=2.3;
             }
           }
         }
       }  
     } else if(mc_ == mc::mc2017){
       if(channel_ == channel::mt){
         if(tau->GetTauID("againstMuonTight3")<0.5) mtau_fakerate_2 = 1.0;
         else if((gm2_==2||gm2_==4)){
           if(fabs(tau->eta()) < 0.4){
             mtau_fakerate_2 = 1.17;
           } else if(fabs(tau->eta()) < 0.8){
             mtau_fakerate_2 = 1.29;
           } else if(fabs(tau->eta()) < 1.2){
             mtau_fakerate_2 = 1.14;
           } else if(fabs(tau->eta()) < 1.7){
             mtau_fakerate_2=0.93;
           } else if(fabs(tau->eta()) < 2.3){
             mtau_fakerate_2=1.61;
           }
           if(tau->decay_mode()==1) mtau_fakerate_2*=0.85;
         }
       } else {
         if(tau->GetTauID("againstMuonLoose3")<0.5) mtau_fakerate_2 = 1.0;
         else if(gm2_==2||gm2_==4){
           if(fabs(tau->eta()) < 0.4){
             mtau_fakerate_2=1.06;
           } else if(fabs(tau->eta()) < 0.8){
             mtau_fakerate_2=1.02;
           } else if(fabs(tau->eta()) < 1.2){
             mtau_fakerate_2=1.10;
           } else if(fabs(tau->eta()) < 1.7){
             mtau_fakerate_2=1.03;
           } else if(fabs(tau->eta()) < 2.3){
             mtau_fakerate_2=1.94;
           }
         }
       }
       if(channel_ == channel::tt){
       unsigned gm1_ = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_1"));
       Tau const* tau1 = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton1"));
         if(tau1->GetTauID("againstMuonLoose3")<0.5) mtau_fakerate_1 = 1.0;
         else if(gm1_==2||gm1_==4){
           if(fabs(tau1->eta()) < 0.4){
             mtau_fakerate_1=1.06;
           } else if(fabs(tau1->eta()) < 0.8){
             mtau_fakerate_1=1.02;
           } else if(fabs(tau1->eta()) < 1.2){
             mtau_fakerate_1=1.10;
           } else if(fabs(tau1->eta()) < 1.7){
             mtau_fakerate_1=1.03;
           } else if(fabs(tau1->eta()) < 2.3){
             mtau_fakerate_1=1.94;
           }
         }
       }  
     } else if(mc_ == mc::mc2018){
       if(channel_ == channel::mt){
         if(tau->GetTauID("againstMuonTight3")<0.5) mtau_fakerate_2 = 1.0;
         else if((gm2_==2||gm2_==4)){
           if(fabs(tau->eta()) < 0.4){
             mtau_fakerate_2 = 1.28;
           } else if(fabs(tau->eta()) < 0.8){
             mtau_fakerate_2 = 1.2;
           } else if(fabs(tau->eta()) < 1.2){
             mtau_fakerate_2 = 1.08;
           } else if(fabs(tau->eta()) < 1.7){
             mtau_fakerate_2=1.0;
           } else if(fabs(tau->eta()) < 2.3){
             mtau_fakerate_2=2.3;
           }
           if(tau->decay_mode()==1) mtau_fakerate_2*=0.85;
         }
       } else {
         if(tau->GetTauID("againstMuonLoose3")<0.5) mtau_fakerate_2 = 1.0;
         else if(gm2_==2||gm2_==4){
           if(fabs(tau->eta()) < 0.4){
             mtau_fakerate_2=1.06;
           } else if(fabs(tau->eta()) < 0.8){
             mtau_fakerate_2=0.96;
           } else if(fabs(tau->eta()) < 1.2){
             mtau_fakerate_2=1.05;
           } else if(fabs(tau->eta()) < 1.7){
             mtau_fakerate_2=1.23;
           } else if(fabs(tau->eta()) < 2.3){
             mtau_fakerate_2=1.19;
           }
         }
       }
       if(channel_ == channel::tt){
       unsigned gm1_ = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_1"));
       Tau const* tau1 = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton1"));
         if(tau1->GetTauID("againstMuonLoose3")<0.5) mtau_fakerate_1 = 1.0;
         else if(gm1_==2||gm1_==4){
           if(fabs(tau1->eta()) < 0.4){
             mtau_fakerate_1=1.06;
           } else if(fabs(tau1->eta()) < 0.8){
             mtau_fakerate_1=0.96;
           } else if(fabs(tau1->eta()) < 1.2){
             mtau_fakerate_1=1.05;
           } else if(fabs(tau1->eta()) < 1.7){
             mtau_fakerate_1=1.23;
           } else if(fabs(tau1->eta()) < 2.3){
             mtau_fakerate_1=1.19;
           }
         }
       }  
     } else{
       if(channel_ == channel::mt){
         if(gm2_==2||gm2_==4){
           if(fabs(tau->eta()) < 0.4){
             mtau_fakerate_2 = 1.5;
           } else if(fabs(tau->eta()) < 0.8){
             mtau_fakerate_2 = 1.4;
           } else if(fabs(tau->eta()) < 1.2){
             mtau_fakerate_2 = 1.21;
           } else if(fabs(tau->eta()) < 1.7){
             mtau_fakerate_2=2.6;
           } else if(fabs(tau->eta()) < 2.3){
             mtau_fakerate_2=2.1;
           }
         }
       } else {
         if(gm2_==2||gm2_==4){
           if(fabs(tau->eta()) < 0.4){
             mtau_fakerate_2=1.15;
           } else if(fabs(tau->eta()) < 0.8){
             mtau_fakerate_2=1.15;
           } else if(fabs(tau->eta()) < 1.2){
             mtau_fakerate_2=1.18;
           } else if(fabs(tau->eta()) < 1.7){
             mtau_fakerate_2=1.2;
           } else if(fabs(tau->eta()) < 2.3){
             mtau_fakerate_2=1.3;
           }
         }
       }
       if(channel_ == channel::tt){
       unsigned gm1_ = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_1"));
       Tau const* tau1 = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton1"));
         if(gm1_==2||gm1_==4){
           if(fabs(tau1->eta()) < 0.4){
             mtau_fakerate_1=1.15;
           } else if(fabs(tau1->eta()) < 0.8){
             mtau_fakerate_1=1.15;
           } else if(fabs(tau1->eta()) < 1.2){
             mtau_fakerate_1=1.18;
           } else if(fabs(tau1->eta()) < 1.7){
             mtau_fakerate_1=1.2;
           } else if(fabs(tau1->eta()) < 2.3){
             mtau_fakerate_1=1.3;
           }
         }
       }
     }
    eventInfo->set_weight("mtau_fakerate",mtau_fakerate_1*mtau_fakerate_2);
   }

   if (do_tau_mode_scale_) {
     Tau const* tau = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton2"));
     if (tau->decay_mode() == 0 && era_ == era::data_2012_rereco) {
       eventInfo->set_weight("tau_mode_scale", 0.88);
     }
   }

    //std::cout <<"----------" << std::endl;
    //eventInfo->print_weights();
    //std::cout << dilepton[0]->GetCandidate("lepton1")->pt() << "    " << dilepton[0]->GetCandidate("lepton1")->eta()  << "    " << dilepton[0]->GetCandidate("lepton2")->pt() << "    " << dilepton[0]->GetCandidate("lepton2")->eta() << std::endl;
    //unsigned gm2_ = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_2"));
    //std::cout << gm2_ << std::endl;
    return 0;
  }

  int HTTWeights::PostAnalysis() {
    return 0;
  }

  void HTTWeights::PrintInfo() {
    ;
  }


  double HTTWeights::Efficiency(double m, double m0, double sigma, double alpha,
    double n, double norm)
  {
    const double sqrtPiOver2 = 1.2533141373;
    const double sqrt2 = 1.4142135624;
    double sig = fabs((double) sigma);
    double t = (m - m0)/sig;
    if(alpha < 0)
      t = -t;
    double absAlpha = fabs(alpha/sig);
    double a = TMath::Power(n/absAlpha,n)*exp(-0.5*absAlpha*absAlpha);
    double b = absAlpha - n/absAlpha;
    double ApproxErf;
    double arg = absAlpha / sqrt2;
    if (arg > 5.) ApproxErf = 1;
    else if (arg < -5.) ApproxErf = -1;
    else ApproxErf = TMath::Erf(arg);
    double leftArea = (1 + ApproxErf) * sqrtPiOver2;
    double rightArea = ( a * 1/TMath::Power(absAlpha - b,n-1)) / (n - 1);
    double area = leftArea + rightArea;
    if( t <= absAlpha ){
      arg = t / sqrt2;
      if(arg > 5.) ApproxErf = 1;
      else if (arg < -5.) ApproxErf = -1;
      else ApproxErf = TMath::Erf(arg);
      return norm * (1 + ApproxErf) * sqrtPiOver2 / area;
    }
    else{
      return norm * (leftArea + a * (1/TMath::Power(t-b,n-1) -
        1/TMath::Power(absAlpha - b,n-1)) / (1 - n)) / area;
    }
  }
}
