#ifndef ICHiggsTauTau_Module_HTTPairSelector_h
#define ICHiggsTauTau_Module_HTTPairSelector_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/interface/HTTConfig.h"


#include <string>

namespace ic {
  
  bool SortBySumPt(CompositeCandidate const* c1, CompositeCandidate const* c2);
  bool SortByIsoET(CompositeCandidate const* c1, CompositeCandidate const* c2, ic::strategy strategy, EventInfo const* eventInfo, std::string tau_idiso_name);
  bool SortByIsoMT(CompositeCandidate const* c1, CompositeCandidate const* c2, ic::strategy strategy, std::string tau_idiso_name);
  bool SortByIsoEM(CompositeCandidate const* c1, CompositeCandidate const* c2, ic::strategy strategy, EventInfo const* eventInfo);
  bool SortByIsoTT(CompositeCandidate const* c1, CompositeCandidate const* c2, std::string tau_idiso_name);


class HTTPairSelector : public ModuleBase {
 private:
  CLASS_MEMBER(HTTPairSelector, ic::channel, channel)
  CLASS_MEMBER(HTTPairSelector, std::string, pair_label)
  CLASS_MEMBER(HTTPairSelector, std::string, met_label)
  CLASS_MEMBER(HTTPairSelector, ic::strategy, strategy) 
  CLASS_MEMBER(HTTPairSelector, bool, mva_met_from_vector)
  CLASS_MEMBER(HTTPairSelector, bool, use_most_isolated)
  CLASS_MEMBER(HTTPairSelector, bool, use_os_preference)
  CLASS_MEMBER(HTTPairSelector, unsigned, faked_tau_selector)
  CLASS_MEMBER(HTTPairSelector, unsigned, hadronic_tau_selector)
  CLASS_MEMBER(HTTPairSelector, unsigned, ztt_mode)
  CLASS_MEMBER(HTTPairSelector, bool, scale_met_for_tau)
  CLASS_MEMBER(HTTPairSelector, double, tau_scale)
  CLASS_MEMBER(HTTPairSelector, bool, use_status_flags)
  CLASS_MEMBER(HTTPairSelector, std::string, allowed_tau_modes)
  CLASS_MEMBER(HTTPairSelector, std::string, gen_taus_label)
  CLASS_MEMBER(HTTPairSelector, fwlite::TFileService*, fs)
  CLASS_MEMBER(HTTPairSelector, unsigned, metcl_mode)
  CLASS_MEMBER(HTTPairSelector, unsigned, metuncl_mode)
  CLASS_MEMBER(HTTPairSelector, bool, shift_jes)
  CLASS_MEMBER(HTTPairSelector, ic::mc, mc)
  CLASS_MEMBER(HTTPairSelector, bool, usePFMET)
  std::vector<Dynamic2DHistoSet *> hists_;
  std::set<int> tau_mode_set_;

 public:
  HTTPairSelector(std::string const& name);
  void CorrectMETForShift(ic::Met * met, ROOT::Math::PxPyPzEVector const& shift);
  virtual ~HTTPairSelector();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();

  std::string tau_idiso_name_;
  

};

}

#endif
