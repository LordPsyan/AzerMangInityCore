/* This file is part of the ScriptDev2 Project. See AUTHORS file for Copyright information
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* ScriptData
SDName: Spell_Scripts
SD%Complete: 100
SDComment: Spell scripts for dummy effects (if script need access/interaction with parts of other AI or instance, add it in related source files)
SDCategory: Spell
EndScriptData */

/* ContentData
spell 19512
spell 21014
spell 21050
spell 26275
spell 29528
spell 29866
spell 34665
spell 37136
spell 39246
spell 44935
spell 45109
spell 45111
EndContentData */

#include "AI/ScriptDevAI/include/sc_common.h"
#include "Spells/Scripts/SpellScript.h"
#include "Grids/GridNotifiers.h"
#include "Grids/GridNotifiersImpl.h"
#include "Grids/CellImpl.h"

/* When you make a spell effect:
- always check spell id and effect index
- always return true when the spell is handled by script
*/

enum
{
    // quest 9452
    SPELL_CAST_FISHING_NET      = 29866,
    GO_RED_SNAPPER              = 181616,
    NPC_ANGRY_MURLOC            = 17102,
    ITEM_RED_SNAPPER            = 23614,
    // SPELL_SUMMON_TEST           = 49214                  // ! Just wrong spell name? It summon correct creature (17102)but does not appear to be used.
};

bool EffectDummyGameObj_spell_dummy_go(Unit* pCaster, uint32 uiSpellId, SpellEffectIndex uiEffIndex, GameObject* pGOTarget, ObjectGuid /*originalCasterGuid*/)
{
    switch (uiSpellId)
    {
        case SPELL_CAST_FISHING_NET:
        {
            if (uiEffIndex == EFFECT_INDEX_0)
            {
                if (pGOTarget->GetRespawnTime() != 0 || pGOTarget->GetEntry() != GO_RED_SNAPPER || pCaster->GetTypeId() != TYPEID_PLAYER)
                    return true;

                if (urand(0, 2))
                {
                    if (Creature* pMurloc = pCaster->SummonCreature(NPC_ANGRY_MURLOC, pCaster->GetPositionX(), pCaster->GetPositionY() + 20.0f, pCaster->GetPositionZ(), 0.0f, TEMPSPAWN_TIMED_OOC_DESPAWN, 10000))
                        pMurloc->AI()->AttackStart(pCaster);
                }
                else
                {
                    if (Item* pItem = ((Player*)pCaster)->StoreNewItemInInventorySlot(ITEM_RED_SNAPPER, 1))
                        ((Player*)pCaster)->SendNewItem(pItem, 1, true, false);
                }

                pGOTarget->SetLootState(GO_JUST_DEACTIVATED);
                return true;
            }
            return true;
        }
    }

    return false;
}

enum
{
    // quest 9629
    SPELL_TAG_MURLOC                    = 30877,
    SPELL_TAG_MURLOC_PROC               = 30875,
    NPC_BLACKSILT_MURLOC                = 17326,
    NPC_TAGGED_MURLOC                   = 17654,

    // quest 9447
    SPELL_HEALING_SALVE                 = 29314,
    SPELL_HEALING_SALVE_DUMMY           = 29319,
    NPC_MAGHAR_GRUNT                    = 16846,

    // quest 10190
    SPELL_RECHARGING_BATTERY            = 34219,
    NPC_DRAINED_PHASE_HUNTER            = 19595,

    // target hulking helboar
    SPELL_ADMINISTER_ANTIDOTE           = 34665,
    NPC_HELBOAR                         = 16880,
    NPC_DREADTUSK                       = 16992,

    // quest 6124/6129
    SPELL_APPLY_SALVE                   = 19512,
    SPELL_SICKLY_AURA                   = 19502,

    NPC_SICKLY_DEER                     = 12298,
    NPC_SICKLY_GAZELLE                  = 12296,

    NPC_CURED_DEER                      = 12299,
    NPC_CURED_GAZELLE                   = 12297,

    // quest 11515
    SPELL_FEL_SIPHON_DUMMY              = 44936,
    NPC_FELBLOOD_INITIATE               = 24918,
    NPC_EMACIATED_FELBLOOD              = 24955,

    // target nestlewood owlkin
    SPELL_INOCULATE_OWLKIN              = 29528,
    NPC_OWLKIN                          = 16518,
    NPC_OWLKIN_INOC                     = 16534,

    // Quest "Disrupt the Greengill Coast" (11541)
    SPELL_ORB_OF_MURLOC_CONTROL         = 45109,
    SPELL_GREENGILL_SLAVE_FREED         = 45110,
    SPELL_ENRAGE                        = 45111,
    NPC_FREED_GREENGILL_SLAVE           = 25085,
    NPC_DARKSPINE_MYRMIDON              = 25060,
    NPC_DARKSPINE_SIREN                 = 25073,

    // quest 9849, item 24501
    SPELL_THROW_GORDAWG_BOULDER         = 32001,
    NPC_MINION_OF_GUROK                 = 18181,

    // npcs that are only interactable while dead
    SPELL_SHROUD_OF_DEATH               = 10848,
    SPELL_SPIRIT_PARTICLES              = 17327,
    NPC_FRANCLORN_FORGEWRIGHT           = 8888,
    NPC_GAERIYAN                        = 9299,

    //  for quest 10584
    SPELL_PROTOVOLTAIC_MAGNETO_COLLECTOR = 37136,
    NPC_ENCASED_ELECTROMENTAL           = 21731,

    // quest 6661
    SPELL_MELODIOUS_RAPTURE             = 21050,
    SPELL_MELODIOUS_RAPTURE_VISUAL      = 21051,
    NPC_DEEPRUN_RAT                     = 13016,
    NPC_ENTHRALLED_DEEPRUN_RAT          = 13017,
};

bool EffectAuraDummy_spell_aura_dummy_npc(const Aura* pAura, bool bApply)
{
    switch (pAura->GetId())
    {
        case SPELL_HEALING_SALVE:
        {
            if (pAura->GetEffIndex() != EFFECT_INDEX_0)
                return true;

            if (bApply)
            {
                if (Unit* pCaster = pAura->GetCaster())
                    pCaster->CastSpell(pAura->GetTarget(), SPELL_HEALING_SALVE_DUMMY, TRIGGERED_OLD_TRIGGERED);
            }

            return true;
        }
        case SPELL_HEALING_SALVE_DUMMY:
        {
            if (pAura->GetEffIndex() != EFFECT_INDEX_0)
                return true;

            if (!bApply)
            {
                Creature* pCreature = (Creature*)pAura->GetTarget();

                pCreature->UpdateEntry(NPC_MAGHAR_GRUNT);

                if (pCreature->getStandState() == UNIT_STAND_STATE_KNEEL)
                    pCreature->SetStandState(UNIT_STAND_STATE_STAND);

                pCreature->ForcedDespawn(60 * IN_MILLISECONDS);
            }

            return true;
        }
        case SPELL_RECHARGING_BATTERY:
        {
            if (pAura->GetEffIndex() != EFFECT_INDEX_0)
                return true;

            if (!bApply)
            {
                if (pAura->GetTarget()->HasAuraState(AURA_STATE_HEALTHLESS_35_PERCENT))
                    ((Creature*)pAura->GetTarget())->UpdateEntry(NPC_DRAINED_PHASE_HUNTER);
            }

            return true;
        }
        case SPELL_TAG_MURLOC:
        {
            Creature* pCreature = (Creature*)pAura->GetTarget();

            if (pAura->GetEffIndex() != EFFECT_INDEX_0)
                return true;

            if (bApply)
            {
                if (pCreature->GetEntry() == NPC_BLACKSILT_MURLOC)
                {
                    if (Unit* pCaster = pAura->GetCaster())
                        pCaster->CastSpell(pCreature, SPELL_TAG_MURLOC_PROC, TRIGGERED_OLD_TRIGGERED);
                }
            }
            else
            {
                if (pCreature->GetEntry() == NPC_TAGGED_MURLOC)
                    pCreature->ForcedDespawn();
            }

            return true;
        }
        case SPELL_ENRAGE:
        {
            if (!bApply || pAura->GetTarget()->GetTypeId() != TYPEID_UNIT)
                return false;

            Creature* pTarget = (Creature*)pAura->GetTarget();

            if (Creature* pCreature = GetClosestCreatureWithEntry(pTarget, NPC_DARKSPINE_MYRMIDON, 25.0f))
            {
                pTarget->AI()->AttackStart(pCreature);
                return true;
            }

            if (Creature* pCreature = GetClosestCreatureWithEntry(pTarget, NPC_DARKSPINE_SIREN, 25.0f))
            {
                pTarget->AI()->AttackStart(pCreature);
                return true;
            }

            return false;
        }
        case SPELL_SHROUD_OF_DEATH:
        case SPELL_SPIRIT_PARTICLES:
        {
            Creature* pCreature = (Creature*)pAura->GetTarget();

            if (!pCreature || (pCreature->GetEntry() != NPC_FRANCLORN_FORGEWRIGHT && pCreature->GetEntry() != NPC_GAERIYAN))
                return false;

            if (bApply)
                pCreature->m_AuraFlags |= UNIT_AURAFLAG_ALIVE_INVISIBLE;
            else
                pCreature->m_AuraFlags &= ~UNIT_AURAFLAG_ALIVE_INVISIBLE;

            return false;
        }
        case SPELL_PROTOVOLTAIC_MAGNETO_COLLECTOR:
        {
            if (pAura->GetEffIndex() != EFFECT_INDEX_0)
                return true;

            Unit* pTarget = pAura->GetTarget();
            if (bApply && pTarget->GetTypeId() == TYPEID_UNIT)
                ((Creature*)pTarget)->UpdateEntry(NPC_ENCASED_ELECTROMENTAL);
            return true;
        }
    }

    return false;
}

bool EffectDummyCreature_spell_dummy_npc(Unit* pCaster, uint32 uiSpellId, SpellEffectIndex uiEffIndex, Creature* pCreatureTarget, ObjectGuid /*originalCasterGuid*/)
{
    switch (uiSpellId)
    {
        case SPELL_ADMINISTER_ANTIDOTE:
        {
            if (uiEffIndex == EFFECT_INDEX_0)
            {
                if (pCreatureTarget->GetEntry() != NPC_HELBOAR)
                    return true;

                // possible needs check for quest state, to not have any effect when quest really complete

                pCreatureTarget->UpdateEntry(NPC_DREADTUSK);
                return true;
            }
            return true;
        }
        case SPELL_APPLY_SALVE:
        {
            if (uiEffIndex == EFFECT_INDEX_0)
            {
                if (pCaster->GetTypeId() != TYPEID_PLAYER)
                    return true;

                if (pCreatureTarget->GetEntry() != NPC_SICKLY_DEER && pCreatureTarget->GetEntry() != NPC_SICKLY_GAZELLE)
                    return true;

                // Update entry, remove aura, set the kill credit and despawn
                uint32 uiUpdateEntry = pCreatureTarget->GetEntry() == NPC_SICKLY_DEER ? NPC_CURED_DEER : NPC_CURED_GAZELLE;
                pCreatureTarget->RemoveAurasDueToSpell(SPELL_SICKLY_AURA);
                pCreatureTarget->UpdateEntry(uiUpdateEntry);
                ((Player*)pCaster)->KilledMonsterCredit(uiUpdateEntry);
                pCreatureTarget->SetImmuneToPlayer(true);
                pCreatureTarget->ForcedDespawn(20000);

                return true;
            }
            return true;
        }
        case SPELL_INOCULATE_OWLKIN:
        {
            if (uiEffIndex == EFFECT_INDEX_0)
            {
                if (pCreatureTarget->GetEntry() != NPC_OWLKIN)
                    return true;

                pCreatureTarget->UpdateEntry(NPC_OWLKIN_INOC);
                pCreatureTarget->AIM_Initialize();
                ((Player*)pCaster)->KilledMonsterCredit(NPC_OWLKIN_INOC);

                // set despawn timer, since we want to remove creature after a short time
                pCreatureTarget->ForcedDespawn(15000);

                return true;
            }
            return true;
        }
        case SPELL_FEL_SIPHON_DUMMY:
        {
            if (uiEffIndex == EFFECT_INDEX_0)
            {
                if (pCreatureTarget->GetEntry() != NPC_FELBLOOD_INITIATE)
                    return true;

                pCreatureTarget->UpdateEntry(NPC_EMACIATED_FELBLOOD);
                return true;
            }
            return true;
        }
        case SPELL_TAG_MURLOC_PROC:
        {
            if (uiEffIndex == EFFECT_INDEX_0)
            {
                if (pCreatureTarget->GetEntry() == NPC_BLACKSILT_MURLOC)
                    pCreatureTarget->UpdateEntry(NPC_TAGGED_MURLOC);
            }
            return true;
        }
        case SPELL_ORB_OF_MURLOC_CONTROL:
        {
            pCreatureTarget->CastSpell(pCaster, SPELL_GREENGILL_SLAVE_FREED, TRIGGERED_OLD_TRIGGERED);

            // Freed Greengill Slave
            pCreatureTarget->UpdateEntry(NPC_FREED_GREENGILL_SLAVE);

            pCreatureTarget->CastSpell(pCreatureTarget, SPELL_ENRAGE, TRIGGERED_OLD_TRIGGERED);

            return true;
        }
        case SPELL_THROW_GORDAWG_BOULDER:
        {
            if (uiEffIndex == EFFECT_INDEX_0)
            {
                for (int i = 0; i < 3; ++i)
                {
                    if (irand(i, 2))                        // 2-3 summons
                        pCreatureTarget->SummonCreature(NPC_MINION_OF_GUROK, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSPAWN_CORPSE_DESPAWN, 5000);
                }

                pCreatureTarget->CastSpell(nullptr, 3617, TRIGGERED_OLD_TRIGGERED); // suicide spell
                return true;
            }
            return true;
        }
        case SPELL_MELODIOUS_RAPTURE:
        {
            if (uiEffIndex == EFFECT_INDEX_0)
            {
                if (pCaster->GetTypeId() != TYPEID_PLAYER && pCreatureTarget->GetEntry() != NPC_DEEPRUN_RAT)
                    return true;

                pCreatureTarget->UpdateEntry(NPC_ENTHRALLED_DEEPRUN_RAT);
                pCreatureTarget->CastSpell(pCreatureTarget, SPELL_MELODIOUS_RAPTURE_VISUAL, TRIGGERED_NONE);
                pCreatureTarget->GetMotionMaster()->MoveFollow(pCaster, frand(0.5f, 3.0f), frand(M_PI_F * 0.8f, M_PI_F * 1.2f));

                ((Player*)pCaster)->KilledMonsterCredit(NPC_ENTHRALLED_DEEPRUN_RAT);
            }
            return true;
        }
    }

    return false;
}

struct GreaterInvisibilityMob : public AuraScript
{
    void OnApply(Aura* aura, bool apply) const override
    {
        if (apply)
            aura->ForcePeriodicity(1 * IN_MILLISECONDS); // tick every second
    }

    void OnPeriodicTickEnd(Aura* aura) const override
    {
        Unit* target = aura->GetTarget();
        if (!target->IsCreature())
            return;

        Creature* invisible = static_cast<Creature*>(target);
        std::list<Unit*> nearbyTargets;
        MaNGOS::AnyUnitInObjectRangeCheck u_check(invisible, float(invisible->GetDetectionRange()));
        MaNGOS::UnitListSearcher<MaNGOS::AnyUnitInObjectRangeCheck> searcher(nearbyTargets, u_check);
        Cell::VisitWorldObjects(invisible, searcher, invisible->GetDetectionRange());
        for (Unit* nearby : nearbyTargets)
        {
            if (invisible->CanAttackOnSight(nearby))
            {
                invisible->AI()->AttackStart(nearby);
                return;
            }
        }
    }
};

struct InebriateRemoval : public AuraScript
{
    void OnApply(Aura* aura, bool apply) const override
    {
        Unit* target = aura->GetTarget();
        if (!target->IsPlayer())
            return;

        SpellEffectIndex effIdx;
        SpellEffectIndex effIdxInebriate;
        switch (aura->GetSpellProto()->Id)
        {
            case 29690: effIdx = EFFECT_INDEX_1; effIdxInebriate = EFFECT_INDEX_2; break;
            case 37591: effIdx = EFFECT_INDEX_0; effIdxInebriate = EFFECT_INDEX_1; break;
            default: return;
        }
        Player* player = static_cast<Player*>(target);
        if (!apply && aura->GetEffIndex() == effIdx)
            player->SetDrunkValue(uint16(std::max(int32(player->GetDrunkValue()) - player->CalculateSpellEffectValue(player, aura->GetSpellProto(), effIdxInebriate) * 256, 0)));
    }
};

struct AstralBite : public SpellScript
{
    void OnEffectExecute(Spell* spell, SpellEffectIndex /*effIdx*/) const override
    {
        if (Unit* caster = spell->GetCaster())
            caster->getThreatManager().modifyAllThreatPercent(-100);
    }
};

struct FelInfusion : public SpellScript
{
    void OnInit(Spell* spell) const override
    {
        spell->SetMaxAffectedTargets(1);
        spell->SetFilteringScheme(EFFECT_INDEX_0, true, SCHEME_CLOSEST);
    }

    bool OnCheckTarget(const Spell* /*spell*/, Unit* target, SpellEffectIndex /*eff*/) const override
    {
        if (!target->IsInCombat())
            return false;
        return true;
    }
};

enum
{
    SPELL_POSSESS       = 32830,
    SPELL_POSSESS_BUFF  = 32831,
    SPELL_POSSESS_INVIS = 32832,
    SPELL_KNOCKDOWN     = 13360,
};

struct AuchenaiPossess : public AuraScript
{
    void OnApply(Aura* aura, bool apply) const override
    {
        if (apply)
        {
            Unit* caster = aura->GetCaster();
            if (caster)
                caster->CastSpell(nullptr, SPELL_POSSESS_INVIS, TRIGGERED_OLD_TRIGGERED);
            aura->GetTarget()->CastSpell(nullptr, SPELL_POSSESS_BUFF, TRIGGERED_OLD_TRIGGERED);
            aura->ForcePeriodicity(1000);
        }
        else
        {
            aura->GetTarget()->RemoveAurasDueToSpell(SPELL_POSSESS_BUFF);
            aura->GetTarget()->CastSpell(aura->GetTarget(), SPELL_KNOCKDOWN, TRIGGERED_OLD_TRIGGERED);
            if (Unit* caster = aura->GetCaster())
                if (caster->IsCreature())
                    static_cast<Creature*>(caster)->ForcedDespawn();
        }
    }

    void OnPeriodicTickEnd(Aura* aura) const override
    {
        if (aura->GetTarget()->GetHealthPercent() < 50.f)
            aura->GetTarget()->RemoveAurasDueToSpell(SPELL_POSSESS);
    }
};

struct GettingSleepyAura : public AuraScript
{
    void OnApply(Aura* aura, bool apply) const override
    {
        if (!apply && aura->GetRemoveMode() == AURA_REMOVE_BY_EXPIRE)
            aura->GetTarget()->CastSpell(nullptr, 34801, TRIGGERED_OLD_TRIGGERED); // Sleep
    }
};

struct AllergiesAura : public AuraScript
{
    void OnApply(Aura* aura, bool apply) const override
    {
        if (apply)
            aura->ForcePeriodicity(10 * IN_MILLISECONDS);
    }

    void OnPeriodicDummy(Aura* aura) const override
    {
        if (urand(0, 2) > 0)
            aura->GetTarget()->CastSpell(nullptr, 31428, TRIGGERED_OLD_TRIGGERED); // Sneeze
    }
};

enum
{
    SPELL_USE_CORPSE = 33985,
};

struct RaiseDead : public SpellScript
{
    bool OnCheckTarget(const Spell* /*spell*/, Unit* target, SpellEffectIndex /*eff*/) const override
    {
        if (!target->IsCreature() || static_cast<Creature*>(target)->HasBeenHitBySpell(SPELL_USE_CORPSE))
            return false;

        return true;
    }
};

struct UseCorpse : public SpellScript
{
    void OnEffectExecute(Spell* spell, SpellEffectIndex /*effIdx*/) const override
    {
        Unit* target = spell->GetUnitTarget();
        if (!target || !target->IsCreature())
            return;

        static_cast<Creature*>(target)->RegisterHitBySpell(SPELL_USE_CORPSE);
    }
};

struct SplitDamage : public SpellScript
{
    void OnEffectExecute(Spell* spell, SpellEffectIndex effIdx) const override
    {
        if (spell->m_spellInfo->Effect[effIdx] != SPELL_EFFECT_SCHOOL_DAMAGE)
            return;

        uint32 count = 0;
        auto& targetList = spell->GetTargetList();
        for (Spell::TargetList::const_iterator ihit = targetList.begin(); ihit != targetList.end(); ++ihit)
            if (ihit->effectHitMask & (1 << effIdx))
                ++count;

        spell->SetDamage(spell->GetDamage() / count); // divide to all targets
    }
};

struct TKDive : public SpellScript
{
    void OnEffectExecute(Spell* spell, SpellEffectIndex effIdx) const override
    {
        if (spell->m_spellInfo->Effect[effIdx] != SPELL_EFFECT_SCHOOL_DAMAGE)
            return;

        Unit* target = spell->GetUnitTarget();
        spell->GetCaster()->AddThreat(target, 1000000.f);
    }
};

/* *****************************
*  PX-238 Winter Wondervolt TRAP
*******************************/
struct WondervoltTrap : public SpellScript
{
    void OnEffectExecute(Spell* spell, SpellEffectIndex effIdx) const override
    {
        if (effIdx == EFFECT_INDEX_0)
        {
            uint32 spells[4] = {26272, 26157, 26273, 26274};    // Four possible transform spells
            if (spell->GetUnitTarget())
                spell->GetUnitTarget()->CastSpell(spell->GetUnitTarget(), spells[urand(0, 3)], TRIGGERED_OLD_TRIGGERED);
        }
    }
};

/* ************************************************************
*  Arcane Cloaking
*  Quests 9121, 9122, 9123, 9378 - Naxxramas, The Dread Citadel
**************************************************************/

struct ArcaneCloaking : public SpellScript
{
    void OnEffectExecute(Spell* spell, SpellEffectIndex effIdx) const override
    {
        if (effIdx == EFFECT_INDEX_0)
        {
            Unit* caster = spell->GetCaster();
            // Naxxramas Entry Flag Effect DND
            if (caster && caster->GetTypeId() == TYPEID_PLAYER)
                caster->CastSpell(caster, 29296, TRIGGERED_OLD_TRIGGERED);  // Cast Naxxramas Entry Flag Trigger DND
        }
    }
};

void AddSC_spell_scripts()
{
    Script* pNewScript = new Script;
    pNewScript->Name = "spell_dummy_go";
    pNewScript->pEffectDummyGO = &EffectDummyGameObj_spell_dummy_go;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "spell_dummy_npc";
    pNewScript->pEffectDummyNPC = &EffectDummyCreature_spell_dummy_npc;
    pNewScript->pEffectAuraDummy = &EffectAuraDummy_spell_aura_dummy_npc;
    pNewScript->RegisterSelf();

    RegisterAuraScript<GreaterInvisibilityMob>("spell_greater_invisibility_mob");
    RegisterAuraScript<InebriateRemoval>("spell_inebriate_removal");
    RegisterSpellScript<AstralBite>("spell_astral_bite");
    RegisterSpellScript<FelInfusion>("spell_fel_infusion");
    RegisterAuraScript<AuchenaiPossess>("spell_auchenai_possess");
    RegisterAuraScript<GettingSleepyAura>("spell_getting_sleepy_aura");
    RegisterAuraScript<AllergiesAura>("spell_allergies");
    RegisterSpellScript<UseCorpse>("spell_use_corpse");
    RegisterSpellScript<RaiseDead>("spell_raise_dead");
    RegisterSpellScript<SplitDamage>("spell_split_damage");
    RegisterSpellScript<TKDive>("spell_tk_dive");
    RegisterSpellScript<WondervoltTrap>("spell_wondervolt_trap");
    RegisterSpellScript<ArcaneCloaking>("spell_arcane_cloaking");
}
