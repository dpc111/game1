require("Assformat")
require("Battle")
require("Fixedformat")
require("MatchAward")
require("Outformat")
require("Switformat")


GlobalCfg = {
   ["Assformat"] = cfg_Assformat,
   ["Battle"] = cfg_Battle,
   ["Fixedformat"] = cfg_Fixedformat,
   ["MatchAward"] = cfg_MatchAward,
   ["Outformat"] = cfg_Outformat,
   ["Switformat"] = cfg_Switformat,
}

function cfg_getCfg(name)
    return GlobalCfg[name]
end

function cfg_getCfgLine(name, index)
    return GlobalCfg[name][index]
end
