
/*
*
*	XYZ
*	File.:	RbxApi.hpp
*	Desc.:	Custom lua functions
*
*/

#pragma once

#include "RbxLua.hpp"
#include "Scheduler.hpp"

#include "../../Utilities/Utils.hpp"
#include "../../Utilities/Hashing/fnv.hpp"

#include "../Misc/D3D.hpp"
#include "../Misc/PointerObfuscation.hpp"


#pragma region Bitwise Operators
typedef int32_t SBits;
typedef uint32_t UBits;

typedef union {
	lua_Number n;
	uint64_t b;
} BitNum;

#define BRET(b)  RL.PushNumber((lua_Number)(SBits)(b)); return 1;

#define BIT_OP(func, opr) \
  static int func(DWORD rL) { syn::RbxLua RL(rL); int i; UBits b = barg(RL, 1); \
    for (i = RL.GetTop(); i > 1; i--) b opr barg(RL, i); BRET(b) }

#define bshl(b, n)  ((b) << (n))
#define bshr(b, n)  ((b) >> (n))
#define bsar(b, n)  ((SBits)(b) >> (n))
#define brol(b, n)  (((b) << (n)) | ((b) >> (32-(n))))
#define bror(b, n)  (((b) << (32-(n))) | ((b) >> (n)))
#define BIT_SH(func, fn) static int func(DWORD rL) { syn::RbxLua RL(rL); UBits b = barg(RL, 1); UBits n = barg(RL, 2) & 31; BRET(fn(b, n)) }
#pragma endregion

namespace syn
{
	extern DWORD InitRL;
	extern DWORD RobloxGlobalRL;
	extern BOOL WSocketEnabled;
	extern BOOL IngameChatEnabled;
	extern BOOL WSocketCreated;
	extern BOOL WSocketChatCreated;
	extern DWORD DataModel;
	extern DWORD LastDataModel;
	extern std::unordered_map<DWORD, DWORD> CClosureHandlers;
	extern HANDLE PipeHandle;
	extern syn::Obfuscation::Member::ObfuscatedMemberString* InitScript;
	extern std::wstring WorkspaceDirectory;
	extern std::string HWID;
	extern std::string WSocketString;
	extern std::string WSocketChatString;
	extern DWORD ModuleSize;
	extern DWORD NtDllSize;
	extern bool SynAutoExec;
	extern bool Teleported;

	using YieldRetFunc = std::function<int(RbxLua)>;

	class RbxYield
	{
	public:
		RbxYield(RbxLua _L);

		int Execute(const std::function<YieldRetFunc()>& YieldedFunction) const;

	private:
		struct RbxThreadRef
		{
			BYTE Pad[8];
			DWORD L;
		};

		RbxLua L = RbxLua(0);
	};

    /* TODO: Why is this a class? It's literally useless, none of this needs to be exposed */
	class RbxApi
	{
	public:
		static __forceinline DWORD GetDataModel() 
		{
			volatile DWORD StackPad[16]{}; // 200 iq
			static DWORD DMPad[16]{}; // MUST be static thanks to MSVCtism
			((DWORD(__thiscall*)(DWORD, DWORD))syn::RobloxBase(OBFUSCATED_NUM(syn::Offsets::DataModel::GetDataModel)))(0, (DWORD)DMPad);
			DWORD DM = DMPad[0];
			if (DM < 0x1000) return 0;
			return DM + 0x44; //vtable bullshit
		}

		static bool checkinstance(RbxLua RL, int Index);

		static bool checksignal(RbxLua RL, int Index);

		static bool checkcframe(RbxLua RL, int Index);

		static int httpget(DWORD rL);

		static int httppost(DWORD rL);

		static int httprequest(DWORD rL);

		static int loadstring(DWORD rL);

		static int getrawmetatable(DWORD rL);

        static int setrawmetatable(DWORD rL);

		static int setreadonly(DWORD rL);

		static int make_writeable(DWORD rL);

		static int make_readonly(DWORD rL);

		static int isreadonly(DWORD rL);

		static int checkcaller(DWORD rL);

		static int setidentity(DWORD rL);

		static int getidentity(DWORD rL);

		static int getreg(DWORD rL);

		static int getgenv(DWORD rL);

		static int getrenv(DWORD rL);

		static int getinstancelist(DWORD rL);

		static int setclipboard(DWORD rL);

		static int getinfo(DWORD rL);

        static int getstack(DWORD rL);

        static int setstack(DWORD rL);

		static int getupvalues(DWORD rL);

		static int getupvalue(DWORD rL);

		static int setupvalue(DWORD rL);

		static int getlocals(DWORD rL);

		static int getlocal(DWORD rL);

		static int setlocal(DWORD rL);

		static int getconstants(DWORD rL);

		static int getconstant(DWORD rL);

		static int setconstant(DWORD rL);

		static int setupvaluename(DWORD rL);

		static int checkrbxlocked(DWORD rL);

		static int checkinst(DWORD rL);

		static int checkparentchain(DWORD rL);

		static int getclassname(DWORD rL);

		static int getstates(DWORD rL);

		static int getpointerfromstate(DWORD rL);

		static int getinstancefromstate(DWORD rL);

		static int getstateenv(DWORD rL);

		static int getloadedmodules(DWORD rL);

		static int getgc(DWORD rL);

		static int getsenv(DWORD rL);

		static int islclosure(DWORD rL);

		static int issynfunc(DWORD rL);

		static int isrbxactive(DWORD rL);

		static int getcallingscript(DWORD rL);

		static int isbeta(DWORD rL);

		static int isluau(DWORD rL);

		static int getnamecallmethod(DWORD rL);

		static int setnamecallmethod(DWORD rL);

		static int getpropvalue(DWORD rL);

		static int setpropvalue(DWORD rL);

#ifdef EnableDebugOutput
		static int showinstance(DWORD rL)
		{
			syn::RbxLua RL(rL);

			const syn::Instance Inst = DereferenceSmartPointerInstance((DWORD) RL.ToUserData(1));

			MessageBox(NULL, IntToHex((DWORD) Inst).c_str(), "XD", MB_OK);
			MessageBox(NULL, Inst.GetInstanceClassName().c_str(), "XD", MB_OK);
			MessageBox(NULL, Inst.GetInstanceName().c_str(), "XD", MB_OK);

			return 0;
		}

		static int crashrbx(DWORD rL)
		{
			*(DWORD*) GetModuleHandle(NULL) = 0;

			return 0;
		}

		static int getnativefunc(DWORD rL)
		{
			syn::RbxLua RL(rL);

			if (RL.Type(1) != R_LUA_TFUNCTION)
				throw std::exception("expected function as argument #1");

			if (!*(BYTE*)(RL.ToPointer(1) + 6))
				throw std::exception("expected C function as argument #1");

			const auto RClosure = (CClosure*) RL.ToPointer(1);
			const auto CFunc = syn::PointerObfuscation::DeObfuscateCClosure((DWORD) RClosure + 20);

			RL.PushInteger(CFunc - (DWORD) syn::RobloxModule + 0x400000U);

			return 1;
		}

		static int decompilesala(DWORD rL)
		{
			syn::RbxLua RL(rL);

			size_t ScriptLength;
			const char* Source = RL.CheckLString(1, &ScriptLength);

			static DWORD HateFlag = 0;
			if (!HateFlag) HateFlag = RbxLua::GetBinValue(FNVA1_CONSTEXPR("hateflag"));
			static DWORD Deserialize = 0;
			if (!Deserialize) Deserialize = RbxLua::GetBinValue(FNVA1_CONSTEXPR("deserialize"));

			const std::string Bytecode = std::string(Source, ScriptLength);
			const DWORD HateBackup = *(DWORD*)HateFlag;
			((int(__cdecl*)(DWORD, const std::string&, const char*, DWORD))Deserialize)(
				rL, Bytecode, OBFUSCATE_STR("SXDecompiler"), 1);
			*(DWORD*)HateFlag = HateBackup;

			const auto Translator = syn::LuaTranslator::GetSingleton();
			const auto Return = Translator->Dump(RL);

			RL.PushLString(Return.c_str(), Return.size());

			return 1;
		}
#endif

#ifdef EnableLuaUTranslator
		static int luaudump(DWORD rL);
#endif

		static int mouse1press(DWORD rL);

		static int mouse1release(DWORD rL);

		static int mouse1click(DWORD rL);

		static int mouse2press(DWORD rL);

		static int mouse2release(DWORD rL);

		static int mouse2click(DWORD rL);

		static int keypress(DWORD rL);

		static int keyrelease(DWORD rL);

		static int mousemoverel(DWORD rL);

		static int mousemoveabs(DWORD rL);

		static int mousescroll(DWORD rL);

		static int newcclosurehandler(DWORD rL);

		static int newcclosurestub(DWORD rL);

		static int newcclosurestub_noupvals(DWORD rL, SynCClosure cL);

		static int newcclosure(DWORD rL);

		static int hookfunction(DWORD rL);

		class RbxProtectedString
		{
			std::string source;
			std::string bytecode;
			DWORD hash = 0;
		};

		static bool decompile_sanity_check(syn::Instance Inst);

		static int decompile(DWORD rL);

		static int dumpstring(DWORD rL);

		static bool file_path_char_good(char c)
		{
			return isalnum(c) || c == '.' || c == ' ' || c == '+' || c == '-' || c == '_' || c == '=' || c == ',' || c == '!' || c == '@' || c == '#' || c == '$' || c == '%' || c == '^' || c == '&' || c == '(' || c == ')' || c == '\\' || c == '/';
		}

		static std::string fix_file_path(const std::string& str, const std::string& extention)
		{
			std::stringstream fstream;
			for (char c : str.substr(0, str.size() - extention.size()))
				if (file_path_char_good(c)) fstream << c;

			return fstream.str() + extention;
		}

		static int readfile(DWORD rL);

		static int loadfile(DWORD rL);

		static bool equals_ignore_case(const std::string& a, const std::string& b)
		{
			return std::equal(a.begin(), a.end(),
				b.begin(), b.end(),
				[](char a, char b) {
				return tolower(a) == tolower(b);
			});
		}

		static int writefile(DWORD rL);

		static int isfile(DWORD rL);

		static int isfolder(DWORD rL);

		static int listfiles(DWORD rL);

		static int makefolder(DWORD rL);

		static int delfolder(DWORD rL);

		static int delfile(DWORD rL);

		static int appendfile(DWORD rL);

		/* connection libraries */
		static int getconnectionshandler(DWORD rL)
		{
			return 0;
		}

		static int getconnections(DWORD rL);

		static int disableconnection(DWORD rL);

		static int enableconnection(DWORD rL);

		static int getconnectionfunc(DWORD rL);

		static int getconnectionstate(DWORD rL);

		static int firesignal(DWORD rL);

		//static int firemessageout(DWORD rL);

		//static int createmsgoutstring(DWORD rL);

		//static int hookerrorhandlers(DWORD rL);

		/*WIP: securelua libraries*/

		static int securelua_gethwid(DWORD rL);

		static int securelua_httpget(DWORD rL);

		static int securelua_randomstring(DWORD rL);

		static int xprotect(DWORD rL);

		/* crypto libraries */
		static int encryptstringcustom(DWORD rL);

		static int encryptstring(DWORD rL);

		static int decryptstringcustom(DWORD rL);

		static int decryptstring(DWORD rL);

		static int hashstring(DWORD rL);

		static int hashstringcustom(DWORD rL);

		static int randomstring(DWORD rL);

		static int derivestring(DWORD rL);

		static int base64encode(DWORD rL);

		static int base64decode(DWORD rL);

		/* render libraries */
		static int createrenderobject(DWORD rL);

		static int setrenderproperty(DWORD rL);

		static int getrenderproperty(DWORD rL);

		static int destroyrenderobject(DWORD rL);

		static int isredirectionenabled(DWORD rL);

		static int printconsole(DWORD rL);

		struct RVector3
		{
			float x, y, z;
		};

		static int getspecialinfo(DWORD rL);

		/* cache libraries */
		static int cacheinvalidate(DWORD rL);

		static int cachereplace(DWORD rL);

		static int iscache(DWORD rL);

		static int setndm(DWORD rL);

		static int getndm(DWORD rL);

		static void reattach_write_pipe(HANDLE Pipe, const std::string& Data)
		{
			DWORD DwWritten;
			WriteFile(Pipe, (Data + "\n").c_str(), Data.size() + 1, &DwWritten, NULL);
			FlushFileBuffers(Pipe);
		}

		static void reportkick_agent(const std::string& PlaceId, const std::string& Message);

		static int reportkick(DWORD rL);

		static void siprun_agent(const std::string& PlaceId);

		static int siprun(DWORD rL);

		/* bit libraries */
		static UBits barg(syn::RbxLua L, int idx)
		{
			BitNum bn;
			UBits b;
			bn.n = L.ToNumber(idx);
			bn.n += 6755399441055744.0;
			b = (UBits)bn.b;
			return b;
		}

		static int bittobit(DWORD rL) { syn::RbxLua RL(rL); BRET(barg(RL, 1)) }
		static int bitbnot(DWORD rL) { syn::RbxLua RL(rL); BRET(~barg(RL, 1)) }

        BIT_OP(bitband, &=)
        BIT_OP(bitbor, |=)
        BIT_OP(bitbxor, ^=)
        BIT_OP(bitbdiv, /=)
        BIT_OP(bitbadd, +=)
        BIT_OP(bitbsub, -=)
        BIT_OP(bitbmul, *=)

        BIT_SH(bitlshift, bshl)
        BIT_SH(bitrshift, bshr)
        BIT_SH(bitarshift, bsar)
        BIT_SH(bitrol, brol)
        BIT_SH(bitror, bror)

		static int bitbswap(DWORD rL);

		static int bittohex(DWORD rL);

		/* console libraries */

		static int messageboxasync(DWORD rL);

		static int setconsolename(DWORD rL);

		static int consoleinputasync(DWORD rL);

		static int consoleprint(DWORD rL);

		static int consoleclear(DWORD rL);

		static int consoleinfo(DWORD rL);

		static int consolewarn(DWORD rL);

		static int consoleerr(DWORD rL);

		static int fireclickdetector(DWORD rL);

		static int firetouch(DWORD rL);

		static int crash(DWORD rL);

		static bool PushLibraries(RbxLua RL, DWORD ORL);

		static void SetDataModel(DWORD DM)
		{
			syn::DataModel = DM;
		}

		static void SetInitScript(std::string Script)
		{
			syn::InitScript = new syn::Obfuscation::Member::ObfuscatedMemberString(Script);
			syn::InitScript->Process();
		}

		static void SetHWID(std::string HWID);
	};
}
