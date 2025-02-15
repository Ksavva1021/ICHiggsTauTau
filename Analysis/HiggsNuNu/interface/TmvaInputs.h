#ifndef ICHiggsTauTau_Module_TmvaInputs_h
#define ICHiggsTauTau_Module_TmvaInputs_h

#include <string>

#include "PhysicsTools/FWLite/interface/TFileService.h"

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"

#include "TTree.h"

namespace ic {
  class TmvaInputs : public ModuleBase {

  private:

    CLASS_MEMBER(TmvaInputs,fwlite::TFileService*, fs);
    CLASS_MEMBER(TmvaInputs,std::string, met_label);
    CLASS_MEMBER(TmvaInputs,std::string, dijet_label);
    CLASS_MEMBER(TmvaInputs,std::string, sel_label);
    CLASS_MEMBER(TmvaInputs,std::string, channel);
    CLASS_MEMBER(TmvaInputs,bool, is_data);
    CLASS_MEMBER(TmvaInputs,bool, is_embedded);

    TTree *outputTree_;
    

    double total_weight_;
    double jet1_pt_;
    double jet2_pt_;
    double jet1_eta_;
    double jet2_eta_;
    double jet1_phi_;
    double jet2_phi_;
    double jet1_E_;
    double jet2_E_;
    double dijet_M_;
    double dijet_deta_;
    double dijet_sumeta_;
    double dijet_dphi_;
    double met_;
    double met_x_;
    double met_y_;
    double met_phi_;
    double met_significance_;
    double sumet_;
    double ht_;
    double mht_;
    double sqrt_ht_;
    double unclustered_et_;
    double unclustered_phi_;
    double jet1met_dphi_;
    double jet2met_dphi_;
    double jetmet_mindphi_;
    double jetunclet_mindphi_;
    double metunclet_dphi_;
    double dijetmet_scalarSum_pt_;
    double dijetmet_vectorialSum_pt_;
    double dijetmet_ptfraction_;
    double jet1met_scalarprod_;
    double jet2met_scalarprod_;
    unsigned n_jets_cjv_30_;
    unsigned n_jets_cjv_20EB_30EE_;

  public:
    TmvaInputs(std::string const& name);
    virtual ~TmvaInputs();
    
    virtual int PreAnalysis();
    virtual int Execute(TreeEvent *event);
    virtual int PostAnalysis();
    virtual void PrintInfo();

   };

}


#endif
