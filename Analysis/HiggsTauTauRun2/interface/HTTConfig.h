#ifndef ICHiggsTauTau_HiggsTauTauRun2_HTTConfig_h
#define ICHiggsTauTau_HiggsTauTauRun2_HTTConfig_h


#include <vector>
#include <set>
#include <map>
#include <string>
#include <iostream>
#include "boost/assign/list_of.hpp"

namespace ic {

inline std::string param_fmt() {
	return "%-25s %-40s\n";
}

inline std::string value_fmt() {
	return "%-20s %-9.2f +/-   %-9.2f";
}


// We define a "type-safe" enum class
// It stops us from doing something like Color c = Shape::square
template<typename def, typename inner = typename def::type>
class safe_enum : public def
{
  typedef typename def::type type;
  inner val;
 
public:
 
  safe_enum(type v) : val(v) {}
  inner underlying() const { return val; }
 
  bool operator == (const safe_enum & s) const { return this->val == s.val; }
  bool operator != (const safe_enum & s) const { return this->val != s.val; }
  bool operator <  (const safe_enum & s) const { return this->val <  s.val; }
  bool operator <= (const safe_enum & s) const { return this->val <= s.val; }
  bool operator >  (const safe_enum & s) const { return this->val >  s.val; }
  bool operator >= (const safe_enum & s) const { return this->val >= s.val; }
};


struct strategy_def {
	enum type {
		paper2013,				// Strategy for the final paper in 2013
		phys14,				// Strategy for running on phys14 samples
        spring15,    //Strategy for running on spring15 samples
        fall15,    //Strategy for running on fall15 samples
        mssmspring16,    //Strategy for running on spring16 samples, MSSM analysis
        smspring16,    //Strategy for running on spring16 samples, SM analysis
        mssmsummer16, //Strategy for running on summer16 samples, MSSM analysis
        smsummer16,    //Strategy for running on summer16 samples, SM analysis
        cpsummer16,
        legacy16,
        cpsummer17,
        cpdecays16,
        cpdecays17,
        cpdecays18
	};
};
typedef safe_enum<strategy_def> strategy;

inline std::string Strategy2String(strategy const& in) {
	static std::map<strategy, std::string> conv = boost::assign::map_list_of
		(strategy::paper2013, 	"paper2013")
		(strategy::phys14, 	"phys14")
                (strategy::spring15, "spring15")
                (strategy::fall15, "fall15")
                (strategy::mssmspring16, "mssmspring16")
                (strategy::smspring16, "smspring16")
                (strategy::mssmsummer16, "mssmsummer16")
                (strategy::smsummer16, "smsummer16")
                (strategy::cpsummer16, "cpsummer16")
                (strategy::legacy16,   "legacy16")
                (strategy::cpsummer17, "cpsummer17")
                (strategy::cpdecays16, "cpdecays16")
                (strategy::cpdecays17, "cpdecays17")
                (strategy::cpdecays18, "cpdecays18");
	if (conv.find(in) != conv.end()) {
		return (conv[in]);
	} else {
		std::cerr << "Enum2String Error, enum not recognised" << std::endl;
		throw;
	}
}

inline strategy String2Strategy(std::string const& in) {
	static std::map<std::string, strategy> conv = boost::assign::map_list_of
	    ("paper2013", 	strategy::paper2013)
	    ("phys14", 	strategy::phys14)
        ("spring15", strategy::spring15)
        ("fall15", strategy::fall15)
        ("mssmspring16", strategy::mssmspring16)
        ("smspring16", strategy::smspring16)
        ("mssmsummer16", strategy::mssmsummer16)
        ("smsummer16", strategy::smsummer16)
        ("cpsummer16", strategy::cpsummer16)
        ("legacy16",   strategy::legacy16)
        ("cpsummer17", strategy::cpsummer17)
        ("cpdecays16", strategy::cpdecays16)
        ("cpdecays17", strategy::cpdecays17)
        ("cpdecays18", strategy::cpdecays18);

	if (conv.find(in) != conv.end()) {
		return (conv.find(in)->second);
	} else {
		std::cerr << "Enum2String Error, string " << in << " not recognised" << std::endl;
		throw;
	}
}

struct era_def {
	enum type {
		data_2011,				// Entire 2011 dataset, mixture of prompt and re-reco
		data_2012_rereco,
		data_2015,
		data_2016,
        data_2017,
        data_2018
	};
};
typedef safe_enum<era_def> era;

inline std::string Era2String(era const& in) {
	static std::map<era, std::string> conv = boost::assign::map_list_of
		(era::data_2011,        "data_2011")
		(era::data_2012_rereco, "data_2012_rereco")
		(era::data_2015,        "data_2015")
		(era::data_2016,        "data_2016")
        (era::data_2017,        "data_2017")
        (era::data_2018,        "data_2018");
	if (conv.find(in) != conv.end()) {
		return (conv[in]);
	} else {
		std::cerr << "Enum2String Error, enum not recognised" << std::endl;
		throw;
	}
}

inline era String2Era(std::string const& in) {
	static std::map<std::string, era> conv = boost::assign::map_list_of
	("data_2011",        era::data_2011)
	("data_2012_rereco", era::data_2012_rereco)
	("data_2015",        era::data_2015)
	("data_2016",        era::data_2016)
    ("data_2017",        era::data_2017)
    ("data_2018",        era::data_2018);
	if (conv.find(in) != conv.end()) {
		return (conv.find(in)->second);
	} else {
		std::cerr << "String2Enum Error, string " << in << " not recognised" << std::endl;
		throw;
	}
}

struct mc_def {
	enum type {
		fall11_42X,				// 42X MC
		summer12_53X,			// 53X MC
		phys14_72X,			// 72X MC
        spring15_74X,      //74X MC 
        fall15_76X,      //76X MC 
        spring16_80X,     //76X MC
        summer16_80X,
        mc2017,
        mc2018,
        mcleg2016
	};
};
typedef safe_enum <mc_def> mc;

inline std::string MC2String(mc const& in) {
	static std::map<mc, std::string> conv = boost::assign::map_list_of
		(mc::fall11_42X,   "fall11_42X")
		(mc::summer12_53X, "summer12_53X")
		(mc::phys14_72X,   "phys14_72X")
        (mc::spring15_74X, "spring15_74X")
        (mc::fall15_76X,   "fall15_76X")
        (mc::spring16_80X, "spring16_80X")
        (mc::summer16_80X, "summer16_80X")
        (mc::mc2017,       "mc2017")
        (mc::mc2018,       "mc2018")
        (mc::mcleg2016,       "mcleg2016"); 

	if (conv.find(in) != conv.end()) {
		return (conv[in]);
	} else {
		std::cerr << "Enum2String Error, enum not recognised" << std::endl;
		throw;
	}
}
inline mc String2MC(std::string const& in) {
	static std::map<std::string, mc> conv = boost::assign::map_list_of
	("fall11_42X",   mc::fall11_42X)
	("summer12_53X", mc::summer12_53X)
	("phys14_72X",   mc::phys14_72X)
    ("spring15_74X", mc::spring15_74X)
    ("fall15_76X",   mc::fall15_76X)
    ("spring16_80X", mc::spring16_80X)
    ("summer16_80X", mc::summer16_80X)
    ("mc2017",       mc::mc2017)
    ("mc2018",       mc::mc2018)
    ("mcleg2016",       mc::mcleg2016);


	if (conv.find(in) != conv.end()) {
		return (conv.find(in)->second);
	} else {
		std::cerr << "String2Enum Error, string " << in << " not recognised" << std::endl;
		throw;
	}
}

struct channel_def {
	enum type {
		et,							
		mt,		
		em,
		tt,
		zee,
		zmm,
        tpzee,
        tpzmm,
        tpmt,
        tpem,
        wmnu,
		mtmet,
		etmet
	};
};
typedef safe_enum<channel_def> channel;

inline std::string Channel2String(channel const& in) {
	static std::map<channel, std::string> conv = boost::assign::map_list_of
		(channel::et, "et")
		(channel::mt, "mt")
		(channel::em, "em")
		(channel::tt, "tt")
		(channel::zee, "zee")
		(channel::zmm, "zmm")
		(channel::tpzee, "tpzee")
		(channel::tpzmm, "tpzmm")
        (channel::tpmt, "tpmt")
        (channel::tpem, "tpem")
		(channel::wmnu, "wmnu")
		(channel::etmet, "etmet")
		(channel::mtmet, "mtmet");

	if (conv.find(in) != conv.end()) {
		return (conv[in]);
	} else {
		std::cerr << "Enum2String Error, enum not recognised" << std::endl;
		throw;
	}
}
inline channel String2Channel(std::string const& in) {
	static std::map<std::string, channel> conv = boost::assign::map_list_of
	("et", channel::et)
	("mt", channel::mt)
	("em", channel::em)
	("tt", channel::tt)
	("zee", channel::zee)
	("zmm", channel::zmm)
	("tpzee", channel::tpzee)
	("tpzmm", channel::tpzmm)
    ("tpmt", channel::tpmt)
    ("tpem", channel::tpem)
	("wmnu", channel::wmnu)
	("etmet", channel::etmet)
	("mtmet", channel::mtmet);

	if (conv.find(in) != conv.end()) {
		return (conv.find(in)->second);
	} else {
		std::cerr << "String2Enum Error, string " << in << " not recognised" << std::endl;
		throw;
	}
}

struct mcorigin_def{
  enum type {
     promptE,
     promptMu,
     tauE,
     tauMu,
     tauHad,
     fake,
     hadE,
     hadMu
  };
};

typedef safe_enum<mcorigin_def> mcorigin;

inline unsigned MCOrigin2UInt(mcorigin const& in) {
  static std::map<mcorigin, unsigned> conv = boost::assign::map_list_of
    (mcorigin::promptE, 1)
    (mcorigin::promptMu, 2)
    (mcorigin::tauE,3)
    (mcorigin::tauMu,4)
    (mcorigin::tauHad,5)
    (mcorigin::fake,6)
    (mcorigin::hadE,7)
    (mcorigin::hadMu,8);

  if (conv.find(in) != conv.end()) {
    return (conv.find(in)->second);
  } else {
    std::cerr << "Enum2UInt error, enum not recognised " <<std::endl;
    throw;
  }
}

inline mcorigin UInt2MCOrigin(unsigned const&in){
   static std::map<unsigned, mcorigin> conv = boost::assign::map_list_of
   (1, mcorigin::promptE)
   (2, mcorigin::promptMu)
   (3, mcorigin::tauE)
   (4, mcorigin::tauMu)
   (5, mcorigin::tauHad)
   (6, mcorigin::fake)
   (7, mcorigin::hadE)
   (8, mcorigin::hadMu);

 if (conv.find(in) != conv.end()){
   return (conv.find(in)->second);
 } else {
   std::cerr << "UInt2Enum Error, unsigned " << in << " not recognised " <<std::endl;
   throw;
  }
} 


inline unsigned JES2UInt(std::string const& in) {
  static std::map<std::string, unsigned> conv = boost::assign::map_list_of
    ("AbsoluteFlavMap",  1)
    ("AbsoluteMPFBias",  2)
    ("AbsoluteScale",    3)
    ("AbsoluteStat",     4)
    ("FlavorQCD",        5)
    ("Fragmentation",    6)
    ("PileUpDataMC",     7)
    ("PileUpPtBB",       8)
    ("PileUpPtEC1",      9)
    ("PileUpPtEC2",      10)
    ("PileUpPtHF",       11)
    ("PileUpPtRef",      12)
    ("RelativeBal",      13)
    ("RelativeFSR",      14)
    ("RelativeJEREC1",   15)
    ("RelativeJEREC2",   16)
    ("RelativeJERHF",    17)
    ("RelativePtBB",     18)
    ("RelativePtEC1",    19)
    ("RelativePtEC2",    20)
    ("RelativePtHF",     21)
    ("RelativeStatEC",   22)
    ("RelativeStatFSR",  23)
    ("RelativeStatHF",   24)
    ("SinglePionECAL",   25)
    ("SinglePionHCAL",   26)
    ("TimePtEta",        27)
    ("Total",            28);

  if (conv.find(in) != conv.end()) {
    return (conv.find(in)->second);
  } else {
    std::cerr << "String2UInt error, string not recognised " <<std::endl;
    throw;
  }
}

inline std::string UInt2JES(unsigned const&in){
   static std::map<unsigned, std::string> conv = boost::assign::map_list_of
    (1,  "AbsoluteFlavMap")
    (2,  "AbsoluteMPFBias")
    (3,  "AbsoluteScale")
    (4,  "AbsoluteStat")
    (5,  "FlavorQCD")
    (6,  "Fragmentation")
    (7,  "PileUpDataMC")
    (8,  "PileUpPtBB")
    (9,  "PileUpPtEC1")
    (10, "PileUpPtEC2")
    (11, "PileUpPtHF")
    (12, "PileUpPtRef")
    (13, "RelativeBal")
    (14, "RelativeFSR")
    (15, "RelativeJEREC1")
    (16, "RelativeJEREC2")
    (17, "RelativeJERHF")
    (18, "RelativePtBB")
    (19, "RelativePtEC1")
    (20, "RelativePtEC2")
    (21, "RelativePtHF")
    (22, "RelativeStatEC")
    (23, "RelativeStatFSR")
    (24, "RelativeStatHF")
    (25, "SinglePionECAL")
    (26, "SinglePionHCAL")
    (27, "TimePtEta")
    (28, "Total");
    
 if (conv.find(in) != conv.end()){
   return (conv.find(in)->second);
 } else {
   std::cerr << "UInt2String Error, unsigned " << in << " not recognised " <<std::endl;
   throw;
  }
} 



}

#endif
