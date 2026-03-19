#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/EventFactor.h"
#include "externals/Pml/WazaData/WazaRankEffect.h"
#include "externals/System/Primitives.h"

namespace Dpr::Battle::Logic::Handler {
    struct Tokusei : ILClass<Tokusei, 0x04c5b2b0> {
        struct HandlerGetFunc : ILClass<HandlerGetFunc, 0x04c5b330> {
            struct Fields : System::MulticastDelegate::Fields {};

            inline void ctor(intptr_t m_target, MethodInfo* method) {
                external<void>(0x017f9150, this, m_target, method);
            }
        };

        struct GET_FUNC_TABLE_ELEM : ILStruct<GET_FUNC_TABLE_ELEM, 0x04c5b328> {
            struct Fields {
                int32_t tokusei;
                HandlerGetFunc::Object* func;
            };

            inline void ctor(int32_t tokusei, HandlerGetFunc::Object* func) {
                external<void>(0x017f9140, this, tokusei, func);
            }
        };

        struct MAX_PRIORITY_PARAM : ILClass<MAX_PRIORITY_PARAM> {
            struct Fields {
                uint8_t pokeID;
                int32_t wazaID;
            };

            inline void ctor() {
                external<void>(0x017f94c0, this);
            }
        };

        struct ultraForce_GetEffectRankTypeTableElem : ILStruct<ultraForce_GetEffectRankTypeTableElem, 0x04c5b338> {
            struct Fields {
                int32_t rankType;
                Dpr::Battle::Logic::BTL_POKEPARAM::ValueID pokeValueID;
            };

            inline void ctor(int32_t rankType, Dpr::Battle::Logic::BTL_POKEPARAM::ValueID pokeValueID) {
                external<void>(0x017f94d0, this, rankType, pokeValueID);
            }
        };

        struct StaticFields {
            GET_FUNC_TABLE_ELEM::Array* GET_FUNC_TABLE;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Ikaku;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Seisinryoku;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Fukutsuno;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_AtuiSibou;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Tikaramoti;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Suisui;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Youryokuso;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Hayaasi;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Tidoriasi;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Harikiri;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Atodasi;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_SlowStart;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Fukugan;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Sunagakure;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Yukigakure;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Iromegane;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_HardRock;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Sniper;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Kasoku;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Tekiouryoku;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Mouka;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Gekiryu;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Sinryoku;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_MusinoSirase;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Konjou;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Plus;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_FlowerGift;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Tousousin;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Technician;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_TetunoKobusi;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Stemi;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_FusiginaUroko;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_SkillLink;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_KairikiBasami;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Surudoime;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_ClearBody;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Tanjun;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_LeafGuard;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Juunan;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Fumin;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_MagumaNoYoroi;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Meneki;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_MizuNoBale;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_MyPace;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Donkan;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_PastelVeil;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Amefurasi;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Hideri;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Sunaokosi;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Sunahaki;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Yukifurasi;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Hajimarinoumi;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Owarinodaiti;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_DeltaStream;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_AirLock;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_IcoBody;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_AmeukeZara;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_SunPower;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Rinpun;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_TennoMegumi;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_UruoiBody;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Dappi;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_PoisonHeal;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_KabutoArmor;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Kyouun;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_IkarinoTubo;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_DokunoToge;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Seidenki;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_HonoNoKarada;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_MeromeroBody;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Housi;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Samehada;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Yuubaku;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_HorobiNoSango;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Hensyoku;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Syncro;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Isiatama;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_NormalSkin;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Trace;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_SizenKaifuku;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_DownLoad;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Yotimu;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_KikenYoti;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Omitoosi;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Ganjou;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Tennen;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Tainetu;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Kansouhada;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_PunkRock;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Tyosui;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Tikuden;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_DenkiEngine;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Kimottama;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Bouon;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Fuyuu;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_FusiginaMamori;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Namake;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Simerike;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Moraibi;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Nightmare;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Nigeasi;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Katayaburi;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Tenkiya;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Yobimizu;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Hiraisin;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Kyuuban;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_HedoroEki;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Bukiyou;
            System::UInt16_array* IgnoreItems_Bukiyou;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Nenchaku;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Pressure;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_MagicGuard;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Akusyuu;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Kagefumi;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Arijigoku;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Jiryoku;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Karuwaza;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Monohiroi;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_TamaHiroi;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_WaruiTeguse;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_NorowareBody;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_KudakeruYoroi;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Tikarazuku;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Makenki;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Katiki;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Yowaki;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_MultiScale;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_FriendGuard;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_IyasiNoKokoro;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Dokubousou;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Netubousou;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Telepassy;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Murakke;
            System::Int32_array* handler_MurakkeTable;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Boujin;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Dokusyu;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_SaiseiRyoku;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Hatomune;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Sunakaki;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_MilacreSkin;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Analyze;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_SunanoTikara;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Surinuke;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_BarrierFree;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_JisinKajou;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_UltraForce;
            ultraForce_GetEffectRankTypeTableElem::Array* RANK_VALUE_TABLE;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_SeiginoKokoro;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Bibiri;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_JyoukiKikan;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Watage;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Miira;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_SamayouTamasii;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Sousyoku;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_ItazuraGokoro;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_MagicMirror;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Syuukaku;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_HeavyMetal;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_LightMetal;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Amanojaku;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Kinchoukan;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_KagakuHenkaGas;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Jukusei;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Kawarimono;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Illusion;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_GoodLuck;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_MentalVeil;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_FlowerVeil;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_SweetVeil;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_MirrorArmor;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_HooBukuro;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_HengenZizai;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_DarkAura;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_FairyAura;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_AuraBreak;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_GanjouAgo;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Gorimuchu;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_FurCoat;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_KusaNoKegawa;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_NumeNume;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_KataiTume;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_SkySkin;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_FairySkin;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_FreezSkin;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_MegaLauncher;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_HayateNoTsubasa;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Boudan;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_OyakoAi;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Magician;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Kyousei;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Zikyuuryoku;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Mizugatame;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Suihou;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Yukikaki;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Haganetukai;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_HaganeNoSeisin;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_UruoiVoice;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_HealingShift;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_ElecSkin;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_SurfTail;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Hitodenasi;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Enkaku;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Zyoounoigen;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_MohuMohu;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_KooriNoRinpun;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Battery;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_PowerSpot;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Receiver;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_TobidasuNakami;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Gyakuzyou;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Nigegosi;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_SoulHeart;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Odoriko;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Husyoku;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_ElecMaker;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_PhychoMaker;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_MistMaker;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_GrassMaker;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Gitai;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Harikomi;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_ZettaiNemuri;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_BrainPrism;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_HutouNoTurugi;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_HukutuNoTate;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_ScrewObire;
        };

        static_assert(sizeof(StaticFields) == 0x710);

        static inline StaticILMethod<0x04c69790, uint8_t*> PTR_Method$$ADD_AirLock {};

        static inline StaticILMethod<0x04c792e0, Dpr::Battle::Logic::EventFactor::EventHandlerArgs::Object**, uint8_t> PTR_Method$$handler_AddSickFailCommon {};
        static inline StaticILMethod<0x04c79538, Dpr::Battle::Logic::EventFactor::EventHandlerArgs::Object**, uint8_t> PTR_Method$$handler_Fumin_InemuriCheck {};


        static inline void handler_AddSickFailCommon(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
            external<void>(0x01946220, args, pokeID);
        }

        static inline void handler_Fumin_InemuriCheck(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
            external<void>(0x01945100, args, pokeID);
        }

        static inline void common_weather_change(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID, uint8_t weather, uint16_t boostItem, bool isPermanent) {
            external<void>(0x01946cc0, args, pokeID, weather, boostItem, isPermanent);
        }
    };
}
