#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/BTL_SICKCONT.h"
#include "externals/Dpr/Battle/Logic/BtlSideEffect.h"
#include "externals/Dpr/Battle/Logic/EventFactor.h"
#include "externals/Dpr/Battle/Logic/EventSystem.h"
#include "externals/System/Primitives.h"

namespace Dpr::Battle::Logic::Handler {
    struct Waza : ILClass<Waza, 0x04c5b348> {
        struct HandlerGetFunc : ILClass<HandlerGetFunc, 0x04c5b448> {
            struct Fields : System::MulticastDelegate::Fields {};

            inline void ctor(intptr_t m_target, MethodInfo* method) {
                external<void>(0x0202bc70, this, m_target, method);
            }
        };

        struct GET_FUNC_TABLE_ELEM : ILStruct<GET_FUNC_TABLE_ELEM> {
            struct Fields {
                int32_t waza;
                HandlerGetFunc::Object* func;
            };

            inline void ctor(int32_t waza, HandlerGetFunc::Object* func) {
                external<void>(0x0202bc50, this, waza, func);
            }
        };

        struct handler_JitabataTableElem : ILStruct<handler_JitabataTableElem> {
            struct Fields {
                uint16_t dot_ratio;
                uint16_t pow;
            };
        };

        struct GetCombiWazaTypeTableElem : ILStruct<GetCombiWazaTypeTableElem> {
            struct Fields {
                int32_t waza1;
                int32_t waza2;
                int32_t effect;
            };
        };

        struct StaticFields {
            GET_FUNC_TABLE_ELEM::Array* GET_FUNC_TABLE;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Texture;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_TrickRoom;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Juryoku;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Kiribarai;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Kawarawari;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Tobigeri;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Monomane;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Sketch;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_KonoyubiTomare;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Ikarinokona;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_KumoNoSu;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_KuroiKiri;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Haneru;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Oiwai;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_TeWoTunagu;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Noroi;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Denjiha;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_NayamiNoTane;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Yumekui;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_TriAttack;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Nettou;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_UtakatanoAria;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Osyaberi;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Makituku;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Uzusio;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_IkariNoMaeba;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Gamusyara;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_TikyuuNage;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Takuwaeru;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Hakidasu;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Nomikomu;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Counter;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_MilerCoat;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_MetalBurst;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Totteoki;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Ibiki;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Fuiuti;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Daibakuhatsu;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Kiaidame;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Juden;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_HorobiNoUta;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_YadorigiNoTane;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_NekoNiKoban;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_AquaRing;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Abareru;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Sawagu;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Korogaru;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_TripleKick;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_GyroBall;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Revenge;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Jitabata;
            handler_JitabataTableElem::Array* handler_JitabataTable;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Karagenki;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Sippegaesi;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Funka;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Siboritoru;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Siomizu;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_RenzokuGiri;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Dameosi;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Ketaguri;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_WeatherBall;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Tatumaki;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Kaminari;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Fubuki;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_ZettaiReido;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Jisin;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_SabakiNoTubute;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_MultiAttack;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_TechnoBaster;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_MezameruPower;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Hatakiotosu;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_MagicCoat;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Dorobou;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Trick;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Naminori;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Fumituke;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_DaiMaxHou;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Mineuti;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Koraeru;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Mamoru;
            System::UInt16_array* WazaTable_Mamoru;
            System::UInt16_array* RandRangeTable_Mamoru;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Recycle;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_PsycoShift;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Itamiwake;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Haradaiko;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Feint;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_IjigenHall;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_TuboWoTuku;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Nemuru;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Meromero;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Texture2;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Encore;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Chouhatu;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Kanasibari;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Present;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Fuuin;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Alomatherapy;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_IyasiNoSuzu;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Okimiyage;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Urami;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_JikoAnji;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_HeartSwap;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_PowerSwap;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_GuardSwap;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_PowerTrick;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_PowerShare;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_GuardShare;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_LockON;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Dokudoku;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Reflector;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_HikariNoKabe;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_SinpiNoMamori;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_SiroiKiri;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Oikaze;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Makibisi;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Dokubisi;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_StealthRock;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_NebaNebaNet;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_WideGuard;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_TatamiGaeshi;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Hensin;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_MikadukiNoMai;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_IyasiNoNegai;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Negaigoto;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Miraiyoti;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_HametuNoNegai;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Ieki;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Narikiri;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_TonboGaeri;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_KousokuSpin;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_BatonTouch;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Teleport;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Nagetukeru;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_DenjiFuyuu;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Tedasuke;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_FukuroDataki;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Nekodamasi;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Deaigasira;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_AsaNoHizasi;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Sunaatume;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_FlowerHeal;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_SoraWoTobu;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_ShadowDive;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Tobihaneru;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Diving;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_AnaWoHoru;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_SolarBeam;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_GodBird;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_RocketZutuki;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Tuibamu;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Hoobaru;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Waruagaki;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Michidure;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Onnen;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Tiisakunaru;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Marukunaru;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Haneyasume;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_KiaiPunch;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_YubiWoFuru;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_SizenNoTikara;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Negoto;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Manekko;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_GensiNoTikara;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_BenomShock;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Tatarime;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Acrobat;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_AsistPower;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_HeavyBomber;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_HeatStamp;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_ElectBall;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_EchoVoice;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Katakiuti;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Ikasama;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_BodyPress;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Mizubitasi;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_MahouNoKona;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_SimpleBeem;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_NakamaDukuri;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_ClearSmog;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Yakitukusu;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_TomoeNage;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Hoeru;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Utiotosu;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_KarawoYaburu;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_MirrorType;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_BodyPurge;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_PsycoShock;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_NasiKuzusi;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_WonderRoom;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_MagicRoom;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Inotigake;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_OsakiniDouzo;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Sakiokuri;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Rinsyou;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_FastGuard;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_SideChange;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_CourtChange;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_InisieNoUta;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Seityou;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_FreezeBolt;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_FlameSoul;
            GetCombiWazaTypeTableElem::Array* CombiTbl;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_CombiWazaCommon;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Halloween;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Morinonoroi;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_FlowerGuard;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_TodomeBari;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_KogoeruHadou;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Hikkurikaesu;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_NeraiPunch;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_SuteZerifu;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_FlyingPress;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_FreezDry;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Souden;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_GrassField;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_MistField;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_ElecField;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_PhychoField;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_KingShield;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Blocking;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_ThousanArrow;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_HappyTime;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_ZibaSousa;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_BenomTrap;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_PlasmaFist;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_FairyLock;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Funjin;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_GeoControl;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_TrickGuard;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_NeedleGuard;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_SouthernWave;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_IjigenRush;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_AuraGuruma;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_DarkHole;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Tootika;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_MezameruDance;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Kahundango;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_CorePunisher;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Kagenui;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Kuraituku;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_TakoGatame;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Zyouka;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Tikarawosuitoru;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Togisumasu;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_SpeedSwap;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Moetukiru;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_KutibasiCanon;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_TrapShell;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Zidanda;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_AuroraVeil;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Saihai;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_MeteorDrive;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_ShadowSteal;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_PhotonGeyser;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Hanabisenyou;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_DaiWall;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_NeraiUti;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_HaisuiNoJin;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_SoulBeat;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Ochakai;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_DengekiKutibasi;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_TarShot;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_DragonArrow;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_InotiNoSizuku;
            Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Newton;
        };

        static inline StaticILMethod<0x04c6a1c0> PTR_Method$$ADD_Karagenki {};
        static inline StaticILMethod<0x04c6a228> PTR_Method$$ADD_KumoNoSu {};
        static inline StaticILMethod<0x04c6a218> PTR_Method$$ADD_Korogaru {};
        static inline StaticILMethod<0x04c6a040> PTR_Method$$ADD_GensiNoTikara {};
        static inline StaticILMethod<0x04c6a488> PTR_Method$$ADD_Siboritoru {};
        static inline StaticILMethod<0x04c6a290> PTR_Method$$ADD_Meromero {};
        static inline StaticILMethod<0x04c6a338> PTR_Method$$ADD_NasiKuzusi {};

        static inline StaticILMethod<0x04c7a3b0, Dpr::Battle::Logic::EventFactor::EventHandlerArgs::Object**, uint8_t> PTR_Method$$handler_Karagenki_WazaPow {};

        static inline StaticILMethod<0x04c7aa20, Dpr::Battle::Logic::EventFactor::EventHandlerArgs::Object**, uint8_t> PTR_Method$$handler_Tobigeri_NoEffect {};

        static inline StaticILMethod<0x04c7a320, Dpr::Battle::Logic::EventFactor::EventHandlerArgs::Object**, uint8_t> PTR_Method$$handler_Jisin_damage {};
        static inline StaticILMethod<0x04c7a318, Dpr::Battle::Logic::EventFactor::EventHandlerArgs::Object**, uint8_t> PTR_Method$$handler_Jisin_checkHide {};

        static inline StaticILMethod<0x04c7a978, Dpr::Battle::Logic::EventFactor::EventHandlerArgs::Object**, uint8_t> PTR_Method$$handler_Tatumaki_checkHide {};

        static_assert(sizeof(StaticFields) == 0x858);

        static inline void common_Korogaru_Unlock(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
            external<void>(0x01803ac0, args, pokeID);
        }

        static inline void removeHandlerForce(EventSystem::Object* pEventSystem, uint8_t pokeID, int32_t waza) {
            external<void>(0x017f99a0, pEventSystem, pokeID, waza);
        }

        static inline bool common_SideEffectCore(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID, int32_t side, BtlSideEffect effect, BTL_SICKCONT::Object* cont, int32_t strType, uint32_t strID, int32_t strArg, bool replaceStrArg0ByExpandSide) {
            return external<bool>(0x018114a0, args, pokeID, side, effect, cont, strType, strID, strArg, replaceStrArg0ByExpandSide);
        }

        static inline bool common_SideEffectStdMsg(EventFactor::EventHandlerArgs::Object** args, uint8_t* pokeID, int32_t side, BtlSideEffect effect, BTL_SICKCONT::Object* cont, uint16_t strID) {
            return external<bool>(0x0180fe10, args, pokeID, side, effect, cont, strID);
        }
    };
}
