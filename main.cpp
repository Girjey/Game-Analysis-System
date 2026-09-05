#define NOMINMAX
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstdio>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <thread>
#include <mutex>
#include <atomic>
#include <cpr/cpr.h>
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include "nlohmann/json.hpp"
#include "header files/CombatEngine.h"
#include "header files/Hero.h"
#include "header files/Enemy.h"
#include "header files/Item.h"
#include "header files/BattleUnit.h"

using json = nlohmann::json;
std::vector<Hero> loadHeroes(const std::string& fp);
std::vector<Enemy> loadEnemies(const std::string& fp);
std::vector<Item> loadItems(const std::string& fp);

struct Overrides {
    double armorScale=100, staminaRegen=4, fatigueThreshold=0.2;
    double fatigueBase=0.3, fatigueRange=0.7, dmgMultiplier=1;
};

void ApplyTheme() {
    ImGuiStyle& s=ImGui::GetStyle(); ImVec4* c=s.Colors;
    s.WindowRounding=s.FrameRounding=s.GrabRounding=s.ChildRounding=s.PopupRounding=s.ScrollbarRounding=s.TabRounding=0;
    s.WindowPadding=ImVec2(10,10); s.FramePadding=ImVec2(6,4); s.ItemSpacing=ImVec2(8,5); s.ScrollbarSize=12;
    ImVec4 bg(0.09f,0.09f,0.09f,1),pn(0.12f,0.12f,0.12f,1),dk(0.07f,0.07f,0.07f,1);
    ImVec4 ac(0.15f,0.35f,0.60f,1),ah(0.20f,0.42f,0.68f,1),aa(0.12f,0.30f,0.55f,1);
    ImVec4 tx(0.72f,0.72f,0.72f,1),td(0.36f,0.36f,0.36f,1),bd(0.18f,0.18f,0.18f,1);
    c[ImGuiCol_Text]=tx;c[ImGuiCol_TextDisabled]=td;c[ImGuiCol_WindowBg]=bg;c[ImGuiCol_ChildBg]=dk;
    c[ImGuiCol_PopupBg]=ImVec4(0.11f,0.11f,0.11f,0.97f);c[ImGuiCol_Border]=bd;c[ImGuiCol_FrameBg]=pn;
    c[ImGuiCol_FrameBgHovered]=ImVec4(0.15f,0.15f,0.15f,1);c[ImGuiCol_FrameBgActive]=ImVec4(0.19f,0.19f,0.19f,1);
    c[ImGuiCol_TitleBg]=dk;c[ImGuiCol_TitleBgActive]=ImVec4(0.09f,0.09f,0.09f,1);
    c[ImGuiCol_ScrollbarBg]=dk;c[ImGuiCol_ScrollbarGrab]=pn;c[ImGuiCol_ScrollbarGrabActive]=ImVec4(0.22f,0.22f,0.22f,1);
    c[ImGuiCol_Button]=ac;c[ImGuiCol_ButtonHovered]=ah;c[ImGuiCol_ButtonActive]=aa;
    c[ImGuiCol_Header]=ImVec4(0.15f,0.15f,0.15f,1);c[ImGuiCol_HeaderHovered]=ImVec4(0.19f,0.19f,0.19f,1);
    c[ImGuiCol_Separator]=bd;c[ImGuiCol_ResizeGrip]=ac;
    c[ImGuiCol_Tab]=ImVec4(0.11f,0.11f,0.11f,1);c[ImGuiCol_TabHovered]=ah;
    c[ImGuiCol_TabSelected]=ac;c[ImGuiCol_TabDimmed]=ImVec4(0.08f,0.08f,0.08f,1);
    c[ImGuiCol_CheckMark]=ac;c[ImGuiCol_SliderGrab]=ac;c[ImGuiCol_SliderGrabActive]=aa;
    c[ImGuiCol_PlotLines]=ImVec4(0.3f,0.7f,1,1);c[ImGuiCol_PlotHistogram]=ac;
    c[ImGuiCol_TableHeaderBg]=ImVec4(0.12f,0.12f,0.12f,1);c[ImGuiCol_TableBorderStrong]=bd;
    c[ImGuiCol_TableBorderLight]=ImVec4(0.14f,0.14f,0.14f,1);c[ImGuiCol_TableRowBgAlt]=ImVec4(0.10f,0.10f,0.10f,1);
}

void SaveHeroToJson(const std::vector<Hero>& h){json d;d["heroes"]=json::array();for(const auto&x:h)d["heroes"].push_back({{"name",x.get_name()},{"hp",x.get_base_hp()},{"physical_damage",x.get_base_physical_damage()},{"magic_damage",x.get_base_magic_damage()},{"crit_damage",x.get_base_crit_damage()},{"crit_chance",x.get_base_crit_chance()},{"defense",x.get_base_defence()},{"magic_resist",x.get_base_magic_resist()},{"accuracy",x.get_base_accuracy()},{"evasion",x.get_base_evasion()},{"stamina",x.get_base_stamina()},{"max_stamina",x.get_base_max_stamina()}});std::ofstream f("hero.json");if(f.is_open()){f<<d.dump(4);f.close();}}
void SaveItemToJson(const std::vector<Item>& items){json d;d["items"]=json::array();for(const auto&x:items)d["items"].push_back({{"name",x.getName()},{"type",x.getType()},{"physical_damage",x.getPhysicalDamage()},{"sharpness",x.getSharpness()},{"magic_damage",x.getMagicDamage()},{"magic_amplification",x.getMagicAmplification()},{"crit_damage",x.getCritDamage()},{"crit_chance",x.getCritChance()},{"attack_speed",x.getAttackSpeed()},{"defense",x.getDef()},{"magic_resist",x.getMagicRest()},{"weight",x.getWeight()},{"durability",x.getDurability()},{"stamina_cost",x.getStaminaCost()}});std::ofstream f("items.json");if(f.is_open()){f<<d.dump(4);f.close();}}
void SaveEnemyToJson(const std::vector<Enemy>& enemies){json d;d["enemies"]=json::array();for(const auto&x:enemies)d["enemies"].push_back({{"name",x.getEnemyName()},{"hp",x.getHp()},{"physical_damage",x.getPhysicalDamage()},{"magic_damage",x.getMagicDamage()},{"armor",x.getArmor()},{"magic_resist",x.getMagicResist()},{"crit_damage",x.getCritDamage()},{"crit_chance",x.getCritChance()},{"accuracy",x.getAccuracy()},{"evasion",x.getEvasion()},{"stamina",x.getStamina()},{"max_stamina",x.getMaxStamina()},{"stamina_cost",x.getStaminaCost()}});std::ofstream f("enemies.json");if(f.is_open()){f<<d.dump(4);f.close();}}

static std::string llmBuf;

std::string BattleToJson(const CombatEngine::BattleAnalysis& a, const CombatEngine* e){
    json r;
    r["result"]={{"fights",a.simCount},{"wins",a.totalWins},{"losses",a.totalLosses},{"win_rate_pct",(double)a.totalWins/a.simCount*100}};
    std::string hN;
    if(!a.allBattlesHistory.empty()&&!a.allBattlesHistory[0].empty())hN=a.allBattlesHistory[0][0].attacker;
    double hDmg=0,eDmg=0,hBlk=0,eBlk=0;int hCr=0,eCr=0,hMi=0,eMi=0,hFat=0,eFat=0,totTurns=0;
    for(const auto& bat:a.allBattlesHistory)for(const auto& l:bat){
        totTurns++;
        if(l.attacker==hN){hDmg+=l.damageDealt;hBlk+=l.armorBlocked;if(l.isCrit)hCr++;if(l.damageDealt==0&&l.turn>0)hMi++;if(l.isFatigued)hFat++;}
        else{eDmg+=l.damageDealt;eBlk+=l.armorBlocked;if(l.isCrit)eCr++;if(l.damageDealt==0&&l.turn>0)eMi++;if(l.isFatigued)eFat++;}}
    int n=a.simCount;
    r["combat"]={{"avg_turns",(double)totTurns/n},{"hero_total_dmg",hDmg},{"enemy_total_dmg",eDmg},
        {"hero_avg_dmg_per_fight",hDmg/n},{"enemy_avg_dmg_per_fight",eDmg/n},
        {"hero_avg_blocked",hBlk/n},{"enemy_avg_blocked",eBlk/n},
        {"hero_crits",hCr},{"enemy_crits",eCr},{"hero_misses",hMi},{"enemy_misses",eMi},
        {"hero_fatigue_turns",hFat},{"enemy_fatigue_turns",eFat}};
    r["hero"]={{"hp",e->get_battle_unit_hp()},{"phys_dmg",e->get_battle_unit_physical_damage()},{"mag_dmg",e->get_battle_unit_magic_damage()},{"crit_dmg",e->get_battle_unit_crit_damage()},{"crit_chance",e->get_battle_unit_chance_crit_damage()},{"armor",e->get_battle_unit_armor()},{"mag_resist",e->get_battle_unit_magic_resist()},{"accuracy",e->get_battle_unit_accuracy()},{"evasion",e->get_battle_unit_evasion()},{"stamina",e->get_battle_unit_max_stamina()}};
    r["enemy"]={{"name",e->get_enemy_name()},{"hp",e->get_enemy_hp()},{"phys_dmg",e->get_enemy_physical_damage()},{"mag_dmg",e->get_enemy_magic_damage()},{"crit_dmg",e->get_enemy_crit_damage()},{"crit_chance",e->get_enemy_chance_crit_damage()},{"armor",e->get_enemy_armor()},{"mag_resist",e->get_enemy_magic_resist()},{"accuracy",e->get_enemy_accuracy()},{"evasion",e->get_enemy_evasion()},{"stamina",e->get_enemy_max_stamina()}};
    return r.dump();
}

void DrawAnalysisTab(CombatEngine* eng, const CombatEngine::BattleAnalysis& analysis, bool hasRes){
    static std::string llmAnalysis;static std::mutex llmMtx;
    static std::thread llmThread;static std::atomic<bool> llmLoading{false},llmDone{false};
    static std::atomic<int> llmTokens{0};static std::string llmError;static char modelName[64]="qwen2.5-coder:8b";
    if(!hasRes||!eng){ImGui::TextDisabled("Run battle analysis first");return;}
    ImGui::InputText("Model##mn",modelName,64);ImGui::Spacing();
    if(!llmLoading&&!llmDone){if(ImGui::Button("ANALYZE BATTLE",ImVec2(-1,28))){
        llmLoading=true;llmDone=false;llmAnalysis.clear();llmError.clear();llmTokens=0;llmBuf.clear();
        if(llmThread.joinable())llmThread.join();
        std::string bd=BattleToJson(analysis,eng);std::string mn=modelName;
        llmThread=std::thread([bd,mn](){
            json body;body["model"]=mn;body["prompt"]=bd;
            body["system"]="Analyze game combat balance from JSON. Respond ONLY in Russian. No markdown, no asterisks, no formatting. Plain text only.\nStructure:\n1. ANALIZ: winrate, damage comparison phys vs magic, crits impact, fatigue role, accuracy and evasion effect. Use numbers from data.\n2. REKOMENDATSII: which stats to change for balance, what is overpowered, what is weak, suggest specific values.\nBe concise. Max 12 sentences total.";
            body["stream"]=true;
            auto resp=cpr::Post(cpr::Url{"http://localhost:11434/api/generate"},cpr::Body{body.dump()},
                cpr::Header{{"Content-Type","application/json"}},
                cpr::WriteCallback{[](std::string data, intptr_t)->bool{
                    llmBuf+=data;size_t p;
                    while((p=llmBuf.find('\n'))!=std::string::npos){
                        std::string ln=llmBuf.substr(0,p);llmBuf=llmBuf.substr(p+1);
                        if(ln.empty())continue;
                        try{auto j=json::parse(ln);
                            if(j.contains("response")&&!j["response"].get<std::string>().empty()){
                                std::lock_guard<std::mutex> lk(llmMtx);llmAnalysis+=j["response"].get<std::string>();llmTokens++;}
                            if(j.value("done",false)){std::lock_guard<std::mutex> lk(llmMtx);llmAnalysis+="\n";}
                        }catch(...){}}return true;}},
                cpr::ConnectTimeout{5000},cpr::Timeout{300000});
            if(resp.status_code!=200){std::lock_guard<std::mutex> lk(llmMtx);llmError="Ollama error: "+std::to_string(resp.status_code)+". Is Ollama running on port 11434?";}
            llmLoading=false;llmDone=true;});}}
    if(llmLoading){float ph=(float)(llmTokens%40)/40.0f;ImGui::ProgressBar(ph,ImVec2(-1,14),"LLM Analyzing...");
        ImGui::TextColored(ImVec4(0.5f,0.8f,1,1),"Tokens: %d...",llmTokens.load());}
    if(!llmError.empty())ImGui::TextColored(ImVec4(1,0.3f,0.3f,1),"%s",llmError.c_str());
    if(!llmAnalysis.empty()||llmDone){
        std::string disp;{std::lock_guard<std::mutex> lk(llmMtx);disp=llmAnalysis;}
        size_t p;
        while((p=disp.find("**"))!=std::string::npos)disp.erase(p,2);
        while((p=disp.find("__"))!=std::string::npos)disp.erase(p,2);
        while((p=disp.find("##"))!=std::string::npos)disp.erase(p,2);
        while((p=disp.find('*'))!=std::string::npos)disp.erase(p,1);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,ImVec2(8,4));
        if(ImGui::BeginChild("AL",ImVec2(0,0))){
            ImGui::TextWrapped("%s",disp.c_str());
            if(llmLoading)ImGui::SetScrollHereY(1.0f);
            ImGui::EndChild();}
        ImGui::PopStyleVar();}
    if(llmDone){ImGui::Spacing();if(ImGui::Button("Clear",ImVec2(-1,22))){llmDone=false;llmAnalysis.clear();llmTokens=0;llmError.clear();}}
}

void DrawRightPanel(CombatEngine*& eng, Overrides& ov, CombatEngine::BattleAnalysis& analysis, bool& hasRes, std::vector<std::string>& bLog) {
    if(!eng){ImGui::TextDisabled("Run analysis to view formulas");return;}
    if(ImGui::BeginTabBar("RT")){
        if(ImGui::BeginTabItem("Formulas")){
            if(ImGui::BeginChild("FC",ImVec2(0,-40))){
                double bA=eng->get_battle_unit_armor(),eA=eng->get_enemy_armor();
                double bAC=ov.armorScale/(ov.armorScale+bA),eAC=ov.armorScale/(ov.armorScale+eA);
                if(ImGui::CollapsingHeader("Armor Coefficient",ImGuiTreeNodeFlags_DefaultOpen)){
                    ImGui::Text("= %.0f / (%.0f + Armor)",ov.armorScale,ov.armorScale);
                    ImGui::TextColored(ImVec4(0.2f,0.85f,0.4f,1),"Hero: %.4f",bAC);
                    ImGui::TextColored(ImVec4(0.95f,0.45f,0.3f,1),"Enemy: %.4f",eAC);
                    ImGui::PushItemWidth(-1);ImGui::InputDouble("##aS",&ov.armorScale,0,0,"%.1f");ImGui::PopItemWidth();
                }
                double bMR=eng->get_battle_unit_magic_resist(),eMR=eng->get_enemy_magic_resist();
                double bMRC=std::max(0.1,1.0-bMR),eMRC=std::max(0.1,1.0-eMR);
                if(ImGui::CollapsingHeader("Magic Resistance")){
                    ImGui::Text("= max(0.1, 1.0 - MagResist)");
                    ImGui::TextColored(ImVec4(0.2f,0.85f,0.4f,1),"Hero: %.4f",bMRC);
                    ImGui::TextColored(ImVec4(0.95f,0.45f,0.3f,1),"Enemy: %.4f",eMRC);
                }
                if(ImGui::CollapsingHeader("Fatigue Multiplier")){
                    ImGui::Text("= %.2f + %.2f * (sta / maxSta)",ov.fatigueBase,ov.fatigueRange);
                    ImGui::TextDisabled("Threshold: %.0f%%",ov.fatigueThreshold*100);
                    ImGui::TextDisabled("Stamina regen/turn: %.1f",ov.staminaRegen);
                    ImGui::PushItemWidth(-1);
                    ImGui::InputDouble("Regen",&ov.staminaRegen,0,0,"%.1f");
                    ImGui::InputDouble("Base",&ov.fatigueBase,0,0,"%.2f");
                    ImGui::InputDouble("Range",&ov.fatigueRange,0,0,"%.2f");
                    ImGui::PopItemWidth();
                }
                if(ImGui::CollapsingHeader("Accuracy vs Evasion")){
                    ImGui::Text("= AtkAcc / (AtkAcc + DefEva)");
                    double hAcc=eng->get_battle_unit_accuracy(),eEva=eng->get_enemy_evasion();
                    double eAcc=eng->get_enemy_accuracy(),hEva=eng->get_battle_unit_evasion();
                    double hH=(hAcc/(hAcc+std::max(1.0,eEva)))*100,eH=(eAcc/(eAcc+std::max(1.0,hEva)))*100;
                    ImGui::TextColored(ImVec4(0.2f,0.85f,0.4f,1),"Hero -> Enemy: %.1f%%",hH);
                    ImGui::TextColored(ImVec4(0.95f,0.45f,0.3f,1),"Enemy -> Hero: %.1f%%",eH);
                }
                if(ImGui::CollapsingHeader("Physical Damage")){
                    ImGui::Text("= (PhysDmg * CritM * FatM) * ArmorC");
                    ImGui::PushItemWidth(-1);ImGui::InputDouble("Global Multiplier",&ov.dmgMultiplier,0,0,"%.3f");ImGui::PopItemWidth();
                }
                if(ImGui::CollapsingHeader("Magic Damage")){
                    ImGui::Text("= (MagDmg * CritM * FatM) * MagResC");
                }
            } ImGui::EndChild();
            if(ImGui::Button("Recalculate",ImVec2(-1,28))){
                BattleUnit bu(Hero("t",eng->get_battle_unit_hp(),eng->get_battle_unit_physical_damage(),eng->get_battle_unit_magic_damage(),eng->get_battle_unit_crit_damage(),eng->get_battle_unit_chance_crit_damage(),eng->get_battle_unit_armor(),eng->get_battle_unit_magic_resist(),eng->get_battle_unit_accuracy(),eng->get_battle_unit_evasion(),eng->get_battle_unit_max_stamina(),eng->get_battle_unit_max_stamina()),Item("t","t",0,1,0,1,0,0,1,0,0,0,100,eng->get_battle_unit_stamina_cost()));
                Enemy en(eng->get_enemy_name(),eng->get_enemy_hp(),eng->get_enemy_physical_damage(),eng->get_enemy_magic_damage(),eng->get_enemy_armor(),eng->get_enemy_magic_resist(),eng->get_enemy_crit_damage(),eng->get_enemy_chance_crit_damage(),eng->get_enemy_accuracy(),eng->get_enemy_evasion(),eng->get_enemy_max_stamina(),eng->get_enemy_max_stamina(),eng->get_enemy_stamina_cost());
                CombatEngine e2(bu,en);analysis=e2.runNewSimulation(analysis.simCount);hasRes=true;
                if(eng)delete eng;eng=new CombatEngine(bu,en);bLog.push_back("[Recalculated]");
            }
            ImGui::EndTabItem();
        }
        if(ImGui::BeginTabItem("LLM Analysis")){
            DrawAnalysisTab(eng,analysis,hasRes);
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
}

void DrawCenterPanel(CombatEngine* eng, bool& hasRes,
    CombatEngine::BattleAnalysis& analysis, std::vector<std::string>& bLog){
    if(!eng){
        ImVec2 sz=ImGui::GetContentRegionAvail();
        ImGui::Dummy(ImVec2(sz.x,sz.y*0.25f));
        float tw=ImGui::CalcTextSize("Run analysis to display battle data").x;
        ImGui::SetCursorPosX((sz.x-tw)*0.5f);
        ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1),"Run analysis to display battle data");
        return;
    }
    if(ImGui::Button("RE-RUN ANALYSIS",ImVec2(-1,32))){
        BattleUnit bu(Hero("t",eng->get_battle_unit_hp(),eng->get_battle_unit_physical_damage(),eng->get_battle_unit_magic_damage(),eng->get_battle_unit_crit_damage(),eng->get_battle_unit_chance_crit_damage(),eng->get_battle_unit_armor(),eng->get_battle_unit_magic_resist(),eng->get_battle_unit_accuracy(),eng->get_battle_unit_evasion(),eng->get_battle_unit_max_stamina(),eng->get_battle_unit_max_stamina()),Item("t","t",0,1,0,1,0,0,1,0,0,0,100,eng->get_battle_unit_stamina_cost()));
        Enemy en(eng->get_enemy_name(),eng->get_enemy_hp(),eng->get_enemy_physical_damage(),eng->get_enemy_magic_damage(),eng->get_enemy_armor(),eng->get_enemy_magic_resist(),eng->get_enemy_crit_damage(),eng->get_enemy_chance_crit_damage(),eng->get_enemy_accuracy(),eng->get_enemy_evasion(),eng->get_enemy_max_stamina(),eng->get_enemy_max_stamina(),eng->get_enemy_stamina_cost());
        CombatEngine e2(bu,en);analysis=e2.runNewSimulation(analysis.simCount);hasRes=true;
        char buf[128];snprintf(buf,128,"[Re-run] %d/%d",analysis.totalWins,analysis.simCount);bLog.push_back(buf);
    }
    if(!hasRes)return;
    ImGui::Spacing();
    float cw=ImGui::GetContentRegionAvail().x;
    float cardH=210.0f,vsW=46.0f,cardW=(cw-vsW)*0.5f;
    auto drawCard=[cardH,cardW](const char* name, ImVec4 col, double hp, double pd, double md, double cd, double cc, double arm, double mr, double acc, double eva, double sta){
        ImGui::PushStyleColor(ImGuiCol_ChildBg,ImVec4(0.10f,0.10f,0.10f,1));
        ImGui::PushStyleColor(ImGuiCol_Border,ImVec4(col.x*0.5f,col.y*0.5f,col.z*0.5f,0.5f));
        if(ImGui::BeginChild(name,ImVec2(cardW,cardH),ImGuiChildFlags_Borders)){
            ImGui::PushStyleColor(ImGuiCol_Text,col);
            float nw=ImGui::CalcTextSize(name).x;
            ImGui::SetCursorPosX((cardW-nw)*0.5f);
            ImGui::Text("%s",name);
            ImGui::PopStyleColor();
            ImGui::Separator();ImGui::Spacing();
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,ImVec2(8,4));
            ImGui::TextColored(ImVec4(0.5f,0.5f,0.5f,1),"%-9s","HP");       ImGui::SameLine(cardW*0.45f);ImGui::Text("%.0f",hp);
            ImGui::TextColored(ImVec4(0.5f,0.5f,0.5f,1),"%-9s","Phys Dmg"); ImGui::SameLine(cardW*0.45f);ImGui::Text("%.0f",pd);
            ImGui::TextColored(ImVec4(0.5f,0.5f,0.5f,1),"%-9s","Mag Dmg");  ImGui::SameLine(cardW*0.45f);ImGui::Text("%.0f",md);
            ImGui::TextColored(ImVec4(0.5f,0.5f,0.5f,1),"%-9s","Crit");     ImGui::SameLine(cardW*0.45f);ImGui::Text("%.1fx (%.0f%%)",cd,cc*100);
            ImGui::TextColored(ImVec4(0.5f,0.5f,0.5f,1),"%-9s","Armor");    ImGui::SameLine(cardW*0.45f);ImGui::Text("%.0f",arm);
            ImGui::TextColored(ImVec4(0.5f,0.5f,0.5f,1),"%-9s","Mag Res");  ImGui::SameLine(cardW*0.45f);ImGui::Text("%.2f",mr);
            ImGui::TextColored(ImVec4(0.5f,0.5f,0.5f,1),"%-9s","Accuracy"); ImGui::SameLine(cardW*0.45f);ImGui::Text("%.0f",acc);
            ImGui::TextColored(ImVec4(0.5f,0.5f,0.5f,1),"%-9s","Evasion");  ImGui::SameLine(cardW*0.45f);ImGui::Text("%.0f",eva);
            ImGui::TextColored(ImVec4(0.5f,0.5f,0.5f,1),"%-9s","Stamina");  ImGui::SameLine(cardW*0.45f);ImGui::Text("%.0f",sta);
            ImGui::PopStyleVar();
            ImGui::EndChild();
        }
        ImGui::PopStyleColor(2);
    };
    if(ImGui::BeginTable("##vs",3,ImGuiTableFlags_SizingFixedFit)){
        ImGui::TableSetupColumn("H",0,cardW);
        ImGui::TableSetupColumn("VS",0,vsW);
        ImGui::TableSetupColumn("E",0,cardW);
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        drawCard("HERO",ImVec4(0.2f,0.85f,0.4f,1),
            eng->get_battle_unit_hp(),eng->get_battle_unit_physical_damage(),eng->get_battle_unit_magic_damage(),
            eng->get_battle_unit_crit_damage(),eng->get_battle_unit_chance_crit_damage(),
            eng->get_battle_unit_armor(),eng->get_battle_unit_magic_resist(),
            eng->get_battle_unit_accuracy(),eng->get_battle_unit_evasion(),eng->get_battle_unit_max_stamina());
        ImGui::TableSetColumnIndex(1);
        float vy=ImGui::GetCursorPosY()+cardH*0.42f;
        ImGui::SetCursorPosY(vy);
        float vxw=ImGui::CalcTextSize("VS").x;
        ImGui::SetCursorPosX(ImGui::GetCursorPosX()+(vsW-vxw)*0.5f);
        ImGui::TextColored(ImVec4(0.9f,0.7f,0.1f,1),"VS");
        ImGui::TableSetColumnIndex(2);
        drawCard(eng->get_enemy_name().c_str(),ImVec4(0.95f,0.45f,0.3f,1),
            eng->get_enemy_hp(),eng->get_enemy_physical_damage(),eng->get_enemy_magic_damage(),
            eng->get_enemy_crit_damage(),eng->get_enemy_chance_crit_damage(),
            eng->get_enemy_armor(),eng->get_enemy_magic_resist(),
            eng->get_enemy_accuracy(),eng->get_enemy_evasion(),eng->get_enemy_max_stamina());
        ImGui::EndTable();
    }
    ImGui::Spacing();ImGui::Separator();ImGui::Spacing();
    ImGui::TextColored(ImVec4(0.3f,0.7f,0.95f,1),"Damage Distribution");
    const int NB=6;const char* bl[NB]={"MISS","1-10","11-25","26-50","51-100","100+"};
    float hB[NB]={},eB[NB]={};
    if(!analysis.allBattlesHistory.empty()&&!analysis.allBattlesHistory[0].empty()){
        std::string hN=analysis.allBattlesHistory[0][0].attacker;
        for(const auto& bat:analysis.allBattlesHistory){
            double lastHHp=0,lastEHp=0;bool got=false;
            for(const auto& l:bat){
                if(!got){
                    if(l.attacker==hN){lastEHp=l.targetHp;got=true;}
                    else{lastHHp=l.targetHp;got=true;}
                }
                double dmg=0;
                if(l.attacker==hN){dmg=lastEHp-l.targetHp;lastEHp=l.targetHp;}
                else{dmg=lastHHp-l.targetHp;lastHHp=l.targetHp;}
                if(dmg<0)dmg=0;
                int bi;
                if(dmg<0.5f)bi=0;
                else if(dmg<=10)bi=1;
                else if(dmg<=25)bi=2;
                else if(dmg<=50)bi=3;
                else if(dmg<=100)bi=4;
                else bi=5;
                if(l.attacker==hN)hB[bi]++;else eB[bi]++;
            }
        }
    }
    float mx=1;
    for(int i=0;i<NB;i++){mx=std::max(mx,std::max(hB[i],eB[i]));}
    float gw=ImGui::GetContentRegionAvail().x;
    float gh=std::min(170.0f,ImGui::GetContentRegionAvail().y-35.0f);
    if(gh<80.0f)gh=80.0f;
    ImVec2 p0=ImGui::GetCursorScreenPos();
    ImGui::Dummy(ImVec2(gw,gh));
    ImDrawList* dl=ImGui::GetWindowDrawList();
    float lm=44.0f,tm=8.0f,bm=30.0f,rm=10.0f;
    float pw=gw-lm-rm,ph=gh-tm-bm;
    dl->AddLine(ImVec2(p0.x+lm,p0.y+tm),ImVec2(p0.x+lm,p0.y+tm+ph),IM_COL32(70,70,70,255));
    dl->AddLine(ImVec2(p0.x+lm,p0.y+tm+ph),ImVec2(p0.x+lm+pw,p0.y+tm+ph),IM_COL32(70,70,70,255));
    for(int g=0;g<=4;g++){
        float y=p0.y+tm+ph-(float)g/4.0f*ph;
        dl->AddLine(ImVec2(p0.x+lm-3,y),ImVec2(p0.x+lm+pw,y),IM_COL32(35,35,35,180));
        char lb[16];snprintf(lb,16,"%.0f",mx*(float)g/4.0f);
        ImVec2 ts=ImGui::CalcTextSize(lb);
        dl->AddText(ImVec2(p0.x+lm-ts.x-5,y-ts.y*0.5f),IM_COL32(120,120,120,255),lb);
    }
    float grpW=pw/(float)NB;
    float barGap=2.0f;
    float bw=grpW*0.24f;
    if(bw>20.0f)bw=20.0f;
    for(int i=0;i<NB;i++){
        float cx=p0.x+lm+(float)i*grpW+grpW*0.5f;
        float hH=(hB[i]/mx)*ph,eH=(eB[i]/mx)*ph;
        if(hH>0.5f)
            dl->AddRectFilled(ImVec2(cx-bw-barGap*0.5f,p0.y+tm+ph-hH),ImVec2(cx-barGap*0.5f,p0.y+tm+ph),IM_COL32(40,180,70,200));
        if(eH>0.5f)
            dl->AddRectFilled(ImVec2(cx+barGap*0.5f,p0.y+tm+ph-eH),ImVec2(cx+bw+barGap*0.5f,p0.y+tm+ph),IM_COL32(200,80,50,200));
        ImVec2 ls=ImGui::CalcTextSize(bl[i]);
        dl->AddText(ImVec2(cx-ls.x*0.5f,p0.y+tm+ph+4),IM_COL32(140,140,140,255),bl[i]);
    }
    float ly=p0.y+tm+ph+20;
    dl->AddRectFilled(ImVec2(p0.x+lm,ly),ImVec2(p0.x+lm+12,ly+8),IM_COL32(40,180,70,220));
    dl->AddText(ImVec2(p0.x+lm+16,ly-1),IM_COL32(170,170,170,255),"Hero");
    dl->AddRectFilled(ImVec2(p0.x+lm+70,ly),ImVec2(p0.x+lm+82,ly+8),IM_COL32(200,80,50,220));
    dl->AddText(ImVec2(p0.x+lm+86,ly-1),IM_COL32(170,170,170,255),"Enemy");
}

void DrawBattleLog(const CombatEngine::BattleAnalysis& a, bool hasRes){
    ImGui::TextColored(ImVec4(0.7f,0.5f,0.9f,1),"Battle Log");ImGui::Separator();
    if(!hasRes){ImGui::TextDisabled("No data");return;}
    float wr=(float)a.totalWins/a.simCount*100;
    ImGui::Text("Fights: %d | Wins: %d | Losses: %d",a.simCount,a.totalWins,a.totalLosses);
    float bw=ImGui::GetContentRegionAvail().x*0.5f;
    ImGui::PushStyleColor(ImGuiCol_PlotHistogram,wr>50?ImVec4(0.2f,0.7f,0.3f,1):ImVec4(0.85f,0.25f,0.2f,1));
    ImGui::ProgressBar(wr/100,ImVec2(bw,16));ImGui::PopStyleColor();
    ImGui::SameLine();ImGui::TextColored(wr>50?ImVec4(0.2f,1,0.3f,1):ImVec4(1,0.3f,0.3f,1),"%.1f%%",wr);
    if(ImGui::BeginChild("BL",ImVec2(0,0))){
        for(size_t b=0;b<a.allBattlesHistory.size();b++){
            bool w=a.battleResults[b];
            ImGui::PushStyleColor(ImGuiCol_Text,w?ImVec4(0.2f,0.9f,0.2f,1):ImVec4(0.9f,0.35f,0.35f,1));
            char h[128];snprintf(h,128,"Fight #%d - %s (%d turns)",(int)b+1,w?"WIN":"LOSS",(int)a.allBattlesHistory[b].size());
            bool op=ImGui::CollapsingHeader(h);ImGui::PopStyleColor();
            if(op){
                double td=0,tb=0;int cr=0,mi=0,ft=0;
                for(const auto& l:a.allBattlesHistory[b]){td+=l.damageDealt;tb+=l.armorBlocked;if(l.isCrit)cr++;if(l.damageDealt==0&&l.turn>0)mi++;if(l.fatigueStarted)ft++;}
                ImGui::TextDisabled("Total dmg: %.0f | Blocked: %.0f | Crits: %d | Misses: %d | Fatigue: %d",td,tb,cr,mi,ft);
                auto fl=ImGuiTableFlags_Borders|ImGuiTableFlags_RowBg|ImGuiTableFlags_SizingFixedFit;
                char t[32];snprintf(t,32,"T%d",(int)b);
                if(ImGui::BeginTable(t,6,fl)){
                    ImGui::TableSetupColumn("#",0,28);ImGui::TableSetupColumn("Attacker",0,65);
                    ImGui::TableSetupColumn("Damage",0,70);ImGui::TableSetupColumn("Blocked",0,60);
                    ImGui::TableSetupColumn("Flags",0,65);ImGui::TableSetupColumn("Target HP",0,80);
                    ImGui::TableHeadersRow();
                    for(const auto& l:a.allBattlesHistory[b]){
                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);ImGui::Text("%d",l.turn);
                        ImGui::TableSetColumnIndex(1);ImGui::Text("%s",l.attacker.c_str());
                        ImGui::TableSetColumnIndex(2);
                        if(l.damageDealt>0){ImGui::TextColored(ImVec4(1,0.85f,0,1),"%.1f",l.damageDealt);if(l.isCrit){ImGui::SameLine();ImGui::TextColored(ImVec4(1,0.15f,0.15f,1),"CRIT");}}
                        else{ImGui::TextDisabled("MISS");}
                        ImGui::TableSetColumnIndex(3);if(l.armorBlocked>0)ImGui::Text("%.1f",l.armorBlocked);
                        ImGui::TableSetColumnIndex(4);
                        if(l.isFatigued)ImGui::TextColored(ImVec4(0.7f,0.4f,0.9f,1),"FATIGUE");
                        if(l.fatigueStarted){ImGui::SameLine();ImGui::TextColored(ImVec4(1,0.5f,0,1),"NEW!");}
                        ImGui::TableSetColumnIndex(5);
                        float r=l.targetHp/300.0f;if(r>1)r=1;if(r<0)r=0;
                        ImGui::PushStyleColor(ImGuiCol_PlotHistogram,ImVec4(1-r,r*0.8f,0,1));
                        char hp[32];snprintf(hp,32,"%.0f",l.targetHp);
                        ImGui::ProgressBar(r,ImVec2(-1,0),hp);ImGui::PopStyleColor();
                    }
                    ImGui::EndTable();
                }
            }
        }
    } ImGui::EndChild();
}

void DrawDynamics(const CombatEngine::BattleAnalysis& a, bool hasRes, const std::string&){
    ImGui::TextColored(ImVec4(0.3f,0.7f,0.95f,1),"Battle Dynamics");ImGui::Separator();
    if(!hasRes||a.allBattlesHistory.empty()){ImGui::TextDisabled("No data");return;}
    if(a.allBattlesHistory[0].empty()){ImGui::TextDisabled("No data");return;}
    std::string hN=a.allBattlesHistory[0][0].attacker;
    size_t mx=0;
    for(const auto& b:a.allBattlesHistory)if(b.size()>mx)mx=b.size();
    if(mx<2){ImGui::TextDisabled("Not enough turns");return;}
    std::vector<float> hHP(mx,0),eHP(mx,0);std::vector<int> cnt(mx,0);
    for(const auto& bat:a.allBattlesHistory){
        if(bat.empty())continue;
        for(const auto& l:bat){
            int i=l.turn-1;if(i<0||i>=(int)mx)continue;cnt[i]++;
            if(l.attacker==hN)eHP[i]+=(float)l.targetHp;
            else hHP[i]+=(float)l.targetHp;
        }
    }
    double hMax=0,eMax=0;
    const auto& fb=a.allBattlesHistory[0];
    for(const auto& l:fb){
        if(l.attacker!=hN&&hMax==0)hMax=l.targetHp;
        if(l.attacker==hN&&eMax==0)eMax=l.targetHp;
    }
    if(hMax==0)hMax=200;if(eMax==0)eMax=200;
    float yMax=(float)std::max(hMax,eMax);
    for(int i=0;i<(int)mx;i++){if(cnt[i]>0){hHP[i]/=cnt[i];eHP[i]/=cnt[i];}}
    static std::vector<float> oH,oE;
    oH.resize(mx);oE.resize(mx);
    for(int i=0;i<(int)mx;i++){oH[i]=hHP[i]/yMax;oE[i]=eHP[i]/yMax;}
    float gw=ImGui::GetContentRegionAvail().x;
    ImGui::TextColored(ImVec4(0.2f,0.85f,0.4f,1),"--- Hero HP");ImGui::SameLine();
    ImGui::TextColored(ImVec4(0.95f,0.45f,0.3f,1),"--- Enemy HP");
    ImGui::PlotLines("##hHP",oH.data(),(int)mx,0,"",0,1.1f,ImVec2(gw,80));
    ImGui::PlotLines("##eHP",oE.data(),(int)mx,0,"",0,1.1f,ImVec2(gw,80));
    ImGui::Spacing();
    double hTot=0,eTot=0;int hCr=0,eCr=0,hMi=0,eMi=0;
    for(const auto& bat:a.allBattlesHistory)for(const auto& l:bat){
        bool ha=(l.attacker==hN);
        if(ha){hTot+=l.damageDealt;if(l.isCrit)hCr++;if(l.damageDealt==0&&l.turn>0)hMi++;}
        else{eTot+=l.damageDealt;if(l.isCrit)eCr++;if(l.damageDealt==0&&l.turn>0)eMi++;}
    }
    int n=a.simCount;
    ImGui::TextColored(ImVec4(0.2f,0.85f,0.4f,1),"Hero: %.0f avg dmg | %d crits | %d misses",hTot/n,hCr,hMi);
    ImGui::TextColored(ImVec4(0.95f,0.45f,0.3f,1),"Enemy: %.0f avg dmg | %d crits | %d misses",eTot/n,eCr,eMi);
    ImGui::Spacing();ImGui::TextDisabled("Avg turns/fight: %.1f",(double)mx);
}

void DrawCombatSimulator(){
    static std::vector<Hero> heroes;static std::vector<Enemy> enemies;static std::vector<Item> weapons;
    static bool inited=false;
    if(!inited){heroes=loadHeroes("hero.json");enemies=loadEnemies("enemies.json");weapons=loadItems("items.json");inited=true;}
    static int custHeroStart=(int)heroes.size(),custWeaponStart=(int)weapons.size(),custEnemyStart=(int)enemies.size();
    static int sH=0,sW=0,sE=0,iters=1;static bool hasRes=false;
    static CombatEngine::BattleAnalysis lastA;static Overrides ov;static CombatEngine* lastE=nullptr;
    static std::string lastHeroName;
    static std::vector<std::string> bLog;
    static char cHN[64]="Custom Hero",cWN[64]="Custom Item",cEN[64]="Custom Enemy";
    static double cH_hp=100,cH_pd=40,cH_md=20,cH_def=15,cH_mr=0.1,cH_cd=2,cH_cc=0.15,cH_acc=100,cH_eva=30,cH_st=100;
    static double cW_pd=50,cW_sh=1,cW_md=0,cW_ma=1,cW_cd=2,cW_cc=0.25,cW_as=1,cW_def=0,cW_mr=0,cW_w=3.5,cW_du=100,cW_sc=2;
    static double cE_hp=100,cE_pd=50,cE_md=0,cE_ar=50,cE_mr=0.25,cE_cd=2,cE_cc=0.1,cE_acc=100,cE_eva=5,cE_st=100,cE_sc=5;
    static float stmr=0;

    ImGuiViewport* vp=ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(vp->WorkPos);ImGui::SetNextWindowSize(vp->WorkSize);ImGui::SetNextWindowViewport(vp->ID);
    ImGui::Begin("##D",nullptr,ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoCollapse|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoBringToFrontOnFocus|ImGuiWindowFlags_NoNavFocus|ImGuiWindowFlags_NoDocking);
    ImGuiID did=ImGui::GetID("MD");
    if(!ImGui::DockBuilderGetNode(did)){
        ImGui::DockBuilderRemoveNode(did);ImGui::DockBuilderAddNode(did,ImGuiDockNodeFlags_DockSpace);
        ImGui::DockBuilderSetNodeSize(did,vp->WorkSize);
        ImGuiID dL,dC,dR,dB,dBL,dDY;
        ImGui::DockBuilderSplitNode(did,ImGuiDir_Down,0.35f,&dB,&dC);
        ImGui::DockBuilderSplitNode(dC,ImGuiDir_Left,0.22f,&dL,&dC);
        ImGui::DockBuilderSplitNode(dC,ImGuiDir_Right,0.33f,&dR,&dC);
        ImGui::DockBuilderSplitNode(dB,ImGuiDir_Left,0.38f,&dBL,&dDY);
        ImGui::DockBuilderDockWindow("Character",dL);ImGui::DockBuilderDockWindow("Formulas",dR);
        ImGui::DockBuilderDockWindow("Battle Log",dBL);ImGui::DockBuilderDockWindow("Dynamics",dDY);
        ImGui::DockBuilderDockWindow("Battle View",dC);ImGui::DockBuilderFinish(did);
    }
    ImGui::DockSpace(did,ImVec2(0,0),ImGuiDockNodeFlags_PassthruCentralNode);ImGui::End();

    // ---- LEFT: Character / Weapon / Enemy ----
    ImGui::Begin("Character");
    if(heroes.empty()||enemies.empty()||weapons.empty()){
        ImGui::TextColored(ImVec4(1,0.2f,0.2f,1),"ERROR: Data not loaded!");
        if(ImGui::Button("Reload"))inited=false;ImGui::End();return;
    }
    if(ImGui::BeginTabBar("HT")){
        if(ImGui::BeginTabItem("Delete")){
            ImGui::TextColored(ImVec4(0.95f,0.35f,0.35f,1),"Delete Builds");ImGui::Separator();ImGui::Spacing();
            static int delH=-1;const char* hl=delH>=0?heroes[delH].get_name().c_str():"Select...";
            if(ImGui::BeginCombo("Hero",hl)){
                for(int i=0;i<(int)heroes.size();i++)if(ImGui::Selectable(heroes[i].get_name().c_str(),delH==i))delH=i;
                ImGui::EndCombo();}
            if(delH>=0&&heroes.size()>1){
                if(ImGui::Button("Delete Hero",ImVec2(-1,24))){
                    if(delH<custHeroStart)custHeroStart--;
                    heroes.erase(heroes.begin()+delH);if(sH>=(int)heroes.size())sH=0;delH=-1;SaveHeroToJson(heroes);stmr=2;}}
            ImGui::Spacing();ImGui::Separator();ImGui::Spacing();
            static int delW=-1;const char* wl=delW>=0?weapons[delW].getName().c_str():"Select...";
            if(ImGui::BeginCombo("Weapon",wl)){
                for(int i=0;i<(int)weapons.size();i++)if(ImGui::Selectable(weapons[i].getName().c_str(),delW==i))delW=i;
                ImGui::EndCombo();}
            if(delW>=0&&weapons.size()>1){
                if(ImGui::Button("Delete Weapon",ImVec2(-1,24))){
                    if(delW<custWeaponStart)custWeaponStart--;
                    weapons.erase(weapons.begin()+delW);if(sW>=(int)weapons.size())sW=0;delW=-1;SaveItemToJson(weapons);stmr=2;}}
            ImGui::Spacing();ImGui::Separator();ImGui::Spacing();
            static int delE=-1;const char* el=delE>=0?enemies[delE].getEnemyName().c_str():"Select...";
            if(ImGui::BeginCombo("Enemy",el)){
                for(int i=0;i<(int)enemies.size();i++)if(ImGui::Selectable(enemies[i].getEnemyName().c_str(),delE==i))delE=i;
                ImGui::EndCombo();}
            if(delE>=0&&enemies.size()>1){
                if(ImGui::Button("Delete Enemy",ImVec2(-1,24))){
                    if(delE<custEnemyStart)custEnemyStart--;
                    enemies.erase(enemies.begin()+delE);if(sE>=(int)enemies.size())sE=0;delE=-1;SaveEnemyToJson(enemies);stmr=2;}}
            if(stmr>0){stmr-=ImGui::GetIO().DeltaTime;ImGui::TextColored(ImVec4(0.2f,0.9f,0.2f,1),"Deleted!");}
            ImGui::EndTabItem();
        }
        if(ImGui::BeginTabItem("Preset")){
            ImGui::TextColored(ImVec4(0.5f,0.7f,0.95f,1),"Hero");
            if(ImGui::BeginCombo("##H",heroes[sH].get_name().c_str())){
                for(int i=0;i<(int)heroes.size();i++)if(ImGui::Selectable(heroes[i].get_name().c_str(),sH==i))sH=i;
                ImGui::EndCombo();}
            const Hero& hh=heroes[sH];ImGui::BeginChild("HS",ImVec2(0,155),false);
            ImGui::Text("HP: %.0f  |  Phys: %.0f  |  Mag: %.0f",hh.get_base_hp(),hh.get_base_physical_damage(),hh.get_base_magic_damage());
            ImGui::Text("Def: %.0f  |  MagRes: %.2f  |  Crit: %.1fx (%.0f%%)",hh.get_base_defence(),hh.get_base_magic_resist(),hh.get_base_crit_damage(),hh.get_base_crit_chance()*100);
            ImGui::Text("Acc: %.0f  |  Eva: %.0f  |  Sta: %.0f/%.0f",hh.get_base_accuracy(),hh.get_base_evasion(),hh.get_base_stamina(),hh.get_base_max_stamina());
            ImGui::EndChild();
            ImGui::Spacing();ImGui::Separator();
            ImGui::TextColored(ImVec4(0.5f,0.7f,0.95f,1),"Weapon");
            if(ImGui::BeginCombo("##W",weapons[sW].getName().c_str())){
                for(int i=0;i<(int)weapons.size();i++)if(ImGui::Selectable(weapons[i].getName().c_str(),sW==i))sW=i;
                ImGui::EndCombo();}
            const Item& wi=weapons[sW];ImGui::TextDisabled("Phys:%.0f Mag:%.0f Crit:%.0f%% Def:+%.0f MR:+%.2f",wi.getPhysicalDamage(),wi.getMagicDamage(),wi.getCritChance()*100,wi.getDef(),wi.getMagicRest());
            ImGui::Spacing();ImGui::Separator();
            ImGui::TextColored(ImVec4(0.95f,0.45f,0.35f,1),"Enemy");
            if(ImGui::BeginCombo("##E",enemies[sE].getEnemyName().c_str())){
                for(int i=0;i<(int)enemies.size();i++)if(ImGui::Selectable(enemies[i].getEnemyName().c_str(),sE==i))sE=i;
                ImGui::EndCombo();}
            const Enemy& en=enemies[sE];ImGui::TextDisabled("HP:%.0f Phys:%.0f Mag:%.0f Armor:%.0f MR:%.2f",en.getHp(),en.getPhysicalDamage(),en.getMagicDamage(),en.getArmor(),en.getMagicResist());
            ImGui::Spacing();ImGui::Separator();
            ImGui::SliderInt("Iterations",&iters,1,500);
            if(ImGui::Button("RUN ANALYSIS",ImVec2(-1,38))){
                Hero hero=heroes[sH];
                BattleUnit bu(hero,weapons[sW]);Enemy target=enemies[sE];CombatEngine engine(bu,target);
                lastA=engine.runNewSimulation(iters);hasRes=true;
                if(lastE)delete lastE;lastE=new CombatEngine(bu,target);
                lastHeroName=hero.get_name();
                char buf[128];snprintf(buf,128,"[%s vs %s] %d/%d (%.1f%%)",hero.get_name().c_str(),target.getEnemyName().c_str(),lastA.totalWins,lastA.simCount,(float)lastA.totalWins/lastA.simCount*100);
                bLog.push_back(std::string(buf));
            }
            ImGui::EndTabItem();
        }
        if(ImGui::BeginTabItem("Custom Hero")){
            static int edH=-1;
            if(ImGui::BeginCombo("##edH",edH<0?"+ New Hero":heroes[edH].get_name().c_str())){
                if(ImGui::Selectable("+ New Hero",edH<0))edH=-1;
                for(int i=custHeroStart;i<(int)heroes.size();i++)
                    if(ImGui::Selectable(heroes[i].get_name().c_str(),edH==i)){
                        edH=i;const Hero& h=heroes[i];strncpy(cHN,h.get_name().c_str(),63);cHN[63]=0;
                        cH_hp=h.get_base_hp();cH_pd=h.get_base_physical_damage();cH_md=h.get_base_magic_damage();
                        cH_def=h.get_base_defence();cH_mr=h.get_base_magic_resist();cH_cd=h.get_base_crit_damage();
                        cH_cc=h.get_base_crit_chance();cH_acc=h.get_base_accuracy();cH_eva=h.get_base_evasion();cH_st=h.get_base_stamina();}
                ImGui::EndCombo();}
            ImGui::InputText("Name",cHN,64);
            ImGui::InputDouble("HP",&cH_hp,10,50,"%.0f");ImGui::InputDouble("Phys Dmg",&cH_pd,5,25,"%.0f");
            ImGui::InputDouble("Mag Dmg",&cH_md,5,25,"%.0f");ImGui::InputDouble("Defense",&cH_def,5,25,"%.0f");
            ImGui::InputDouble("Mag Resist",&cH_mr,0.05,0.1,"%.2f");ImGui::InputDouble("Crit Dmg",&cH_cd,0.25,0.5,"%.2f");
            ImGui::InputDouble("Crit Chance",&cH_cc,0.01,0.05,"%.2f");ImGui::InputDouble("Accuracy",&cH_acc,5,25,"%.0f");
            ImGui::InputDouble("Evasion",&cH_eva,5,10,"%.0f");ImGui::InputDouble("Stamina",&cH_st,10,50,"%.0f");
            if(ImGui::Button(edH<0?"Create Hero":"Update Hero",ImVec2(-1,24))){
                Hero nh(cHN,cH_hp,cH_pd,cH_md,cH_cd,cH_cc,cH_def,cH_mr,cH_acc,cH_eva,cH_st,cH_st);
                if(edH<0){heroes.push_back(nh);sH=(int)heroes.size()-1;}
                else{heroes[edH]=nh;}
                SaveHeroToJson(heroes);stmr=2;}
            if(stmr>0){stmr-=ImGui::GetIO().DeltaTime;ImGui::TextColored(ImVec4(0.2f,0.9f,0.2f,1),edH<0?"Hero created!":"Hero updated!");}
            ImGui::EndTabItem();
        }
        if(ImGui::BeginTabItem("Custom Weapon")){
            static int edW=-1;static char cWT[32]="weapon";
            if(ImGui::BeginCombo("##edW",edW<0?"+ New Weapon":weapons[edW].getName().c_str())){
                if(ImGui::Selectable("+ New Weapon",edW<0))edW=-1;
                for(int i=custWeaponStart;i<(int)weapons.size();i++)
                    if(ImGui::Selectable(weapons[i].getName().c_str(),edW==i)){
                        edW=i;const Item& w=weapons[i];strncpy(cWN,w.getName().c_str(),63);cWN[63]=0;
                        strncpy(cWT,w.getType().c_str(),31);cWT[31]=0;
                        cW_pd=w.getPhysicalDamage();cW_sh=w.getSharpness();cW_md=w.getMagicDamage();cW_ma=w.getMagicAmplification();
                        cW_cd=w.getCritDamage();cW_cc=w.getCritChance();cW_as=w.getAttackSpeed();cW_def=w.getDef();
                        cW_mr=w.getMagicRest();cW_w=w.getWeight();cW_du=w.getDurability();cW_sc=w.getStaminaCost();}
                ImGui::EndCombo();}
            ImGui::InputText("Name",cWN,64);ImGui::InputText("Type",cWT,32);
            ImGui::InputDouble("Phys Dmg",&cW_pd,5,25,"%.1f");ImGui::InputDouble("Sharpness",&cW_sh,0.1,0.5,"%.2f");
            ImGui::InputDouble("Mag Dmg",&cW_md,5,25,"%.1f");ImGui::InputDouble("Mag Amplify",&cW_ma,0.1,0.5,"%.2f");
            ImGui::InputDouble("Crit Dmg",&cW_cd,0.25,0.5,"%.2f");ImGui::InputDouble("Crit Chance",&cW_cc,0.01,0.05,"%.2f");
            ImGui::InputDouble("Atk Speed",&cW_as,0.1,0.5,"%.2f");ImGui::InputDouble("Defense",&cW_def,5,25,"%.1f");
            ImGui::InputDouble("Mag Resist",&cW_mr,0.05,0.1,"%.2f");ImGui::InputDouble("Weight",&cW_w,0.5,1,"%.1f");
            ImGui::InputDouble("Durability",&cW_du,10,50,"%.0f");ImGui::InputDouble("Stamina Cost",&cW_sc,0.5,1,"%.1f");
            if(ImGui::Button(edW<0?"Create Weapon":"Update Weapon",ImVec2(-1,24))){
                Item nw(cWN,cWT,cW_pd,cW_sh,cW_md,cW_ma,cW_cd,cW_cc,cW_as,cW_def,cW_mr,cW_w,cW_du,cW_sc);
                if(edW<0){weapons.push_back(nw);sW=(int)weapons.size()-1;}
                else{weapons[edW]=nw;}
                SaveItemToJson(weapons);stmr=2;}
            if(stmr>0){stmr-=ImGui::GetIO().DeltaTime;ImGui::TextColored(ImVec4(0.2f,0.9f,0.2f,1),edW<0?"Weapon created!":"Weapon updated!");}
            ImGui::EndTabItem();
        }
        if(ImGui::BeginTabItem("Custom Enemy")){
            static int edE=-1;
            if(ImGui::BeginCombo("##edE",edE<0?"+ New Enemy":enemies[edE].getEnemyName().c_str())){
                if(ImGui::Selectable("+ New Enemy",edE<0))edE=-1;
                for(int i=custEnemyStart;i<(int)enemies.size();i++)
                    if(ImGui::Selectable(enemies[i].getEnemyName().c_str(),edE==i)){
                        edE=i;const Enemy& e=enemies[i];strncpy(cEN,e.getEnemyName().c_str(),63);cEN[63]=0;
                        cE_hp=e.getHp();cE_pd=e.getPhysicalDamage();cE_md=e.getMagicDamage();
                        cE_ar=e.getArmor();cE_mr=e.getMagicResist();cE_cd=e.getCritDamage();
                        cE_cc=e.getCritChance();cE_acc=e.getAccuracy();cE_eva=e.getEvasion();
                        cE_st=e.getStamina();cE_sc=e.getStaminaCost();}
                ImGui::EndCombo();}
            ImGui::InputText("Name",cEN,64);
            ImGui::InputDouble("HP",&cE_hp,10,50,"%.0f");ImGui::InputDouble("Phys Dmg",&cE_pd,5,25,"%.0f");
            ImGui::InputDouble("Mag Dmg",&cE_md,5,25,"%.0f");ImGui::InputDouble("Armor",&cE_ar,5,25,"%.0f");
            ImGui::InputDouble("Mag Resist",&cE_mr,0.05,0.1,"%.2f");ImGui::InputDouble("Crit Dmg",&cE_cd,0.25,0.5,"%.2f");
            ImGui::InputDouble("Crit Chance",&cE_cc,0.01,0.05,"%.2f");ImGui::InputDouble("Accuracy",&cE_acc,5,25,"%.0f");
            ImGui::InputDouble("Evasion",&cE_eva,5,10,"%.0f");ImGui::InputDouble("Stamina",&cE_st,10,50,"%.0f");
            ImGui::InputDouble("Stamina Cost",&cE_sc,1,5,"%.0f");
            if(ImGui::Button(edE<0?"Create Enemy":"Update Enemy",ImVec2(-1,24))){
                Enemy ne(cEN,cE_hp,cE_pd,cE_md,cE_ar,cE_mr,cE_cd,cE_cc,cE_acc,cE_eva,cE_st,cE_st,cE_sc);
                if(edE<0){enemies.push_back(ne);sE=(int)enemies.size()-1;}
                else{enemies[edE]=ne;}
                SaveEnemyToJson(enemies);stmr=2;}
            if(stmr>0){stmr-=ImGui::GetIO().DeltaTime;ImGui::TextColored(ImVec4(0.2f,0.9f,0.2f,1),edE<0?"Enemy created!":"Enemy updated!");}
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    ImGui::End();

    ImGui::Begin("Battle View");DrawCenterPanel(lastE,hasRes,lastA,bLog);ImGui::End();
    ImGui::Begin("Formulas");DrawRightPanel(lastE,ov,lastA,hasRes,bLog);ImGui::End();
    ImGui::Begin("Battle Log");DrawBattleLog(lastA,hasRes);ImGui::End();
    ImGui::Begin("Dynamics");DrawDynamics(lastA,hasRes,lastHeroName);ImGui::End();
}

int main(){
    if(!glfwInit())return 1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* w=glfwCreateWindow(1600,900,"Game Analysis System",nullptr,nullptr);
    if(!w)return 1;glfwMakeContextCurrent(w);glfwSwapInterval(1);
    IMGUI_CHECKVERSION();ImGui::CreateContext();
    ImGuiIO& io=ImGui::GetIO();io.ConfigFlags|=ImGuiConfigFlags_DockingEnable;
    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf",18.0f,NULL,io.Fonts->GetGlyphRangesCyrillic());
    ApplyTheme();
    ImGui_ImplGlfw_InitForOpenGL(w,true);ImGui_ImplOpenGL3_Init("#version 130");
    while(!glfwWindowShouldClose(w)){
        glfwPollEvents();ImGui_ImplOpenGL3_NewFrame();ImGui_ImplGlfw_NewFrame();ImGui::NewFrame();
        DrawCombatSimulator();ImGui::Render();
        int dw,dh;glfwGetFramebufferSize(w,&dw,&dh);glViewport(0,0,dw,dh);
        glClearColor(0.05f,0.05f,0.05f,1);glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(w);
    }
    ImGui_ImplOpenGL3_Shutdown();ImGui_ImplGlfw_Shutdown();ImGui::DestroyContext();
    glfwDestroyWindow(w);glfwTerminate();return 0;
}
