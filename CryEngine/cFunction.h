#include <windows.h>
#include <windowsx.h>
#include <iostream>
#include <tchar.h>
#include <xstring>
#include <string> 
#include <thread>


void WriteLogFile(const char* LogText, ...)
{
	FILE* MyLog;
	fopen_s(&MyLog, "C://Hello//LOGFILE.log", "a");


	char Buffer[1024];
	va_list ArgList;
	va_start(ArgList, LogText);
	vsnprintf_s(Buffer, 1023, LogText, ArgList);
	va_end(ArgList);

	if (MyLog)
	{
		fprintf(MyLog, "%s", Buffer);
		fclose(MyLog);
	}
}

void Log(const char* LogText, ...)
{
	char Buffer[1024];
	va_list ArgList;
	va_start(ArgList, LogText);
	vsnprintf_s(Buffer, 1023, LogText, ArgList);
	va_end(ArgList);

	WriteLogFile("%s\n", Buffer);
}


enum ERWIFlagsShot
{
	eAmmoFlags = 0x31F,
	eAmmoWallShot = 0x10,
	eAmmoWallShotF = 0x75,
	eAmmoWallShotE = 0x7E
};

enum entity_object_flags
{
	flag_weapons = 0x480012
};

enum EIDWeapons
{
	Rifle = 1818650994,
	Pistol = 1953720688,
	Knife = 1718185579,
	Grenade = 1852142183,
	AmmoBox = 1869442401,
	MedicineChest = 1801741677,
	Defibrillator = 26212,
	ArmorPlate = 1634755954,
	Mine = 30574
};
enum EClassId
{
	Sturm = 0,
	Medic = 3,
	Engineer = 4,
	Sniper = 2
};


Vec3 TargetPos = ZERO;
f32 PriorityLimit = 5000.0f;

BOOL IsMyTeam(INT mTeam, INT pTeam) { return !(mTeam != pTeam || pTeam == 0); }
BOOL IsMyTeam(INT mTeam, IActor* pActor) { return !(mTeam != pActor->TeamId || pActor->TeamId == 0); }
bool IsMyTeam(IActor* mActor, IActor* pActor) { return !(mActor->TeamId != pActor->TeamId || pActor->TeamId == 0); }

Vec3 GetMPos(Vec3 vBonePos, Vec3 CamPos, FLOAT popravka)
{
	Vec3 vOut = vBonePos - CamPos;
	FLOAT sqr = (FLOAT)_sqrt(vOut.x * vOut.x + vOut.y * vOut.y + vOut.z * vOut.z);
	vOut /= sqr;
	vOut *= sqr - popravka;
	return vOut;
}
bool is_Visible(Vec3 vBonePos, FLOAT popravka)
{
	ray_hit tmpHit;
	Vec3 CamPos = gRender->vPos;
	Vec3 vOut(GetMPos(vBonePos, CamPos, popravka));
	return !gEnv->pPhysicalWorld->RayWorldIntersection(CamPos, vOut, 0x100 | 1, 0xA | 0x400, &tmpHit, 1);
}
bool Priority(Vec3 vPos)
{
	FLOAT EntityDist = gRender->GetHypotenuseFromCenter(vPos);
	if (EntityDist > PriorityLimit) return FALSE;
	PriorityLimit = EntityDist;
	return TRUE;
}
void Aim(IWeaponPTR* pWeaponPTR, const char* Name, const char* ClassName, IActor* m_pActor, IItem* m_pItem, Vec3 vBonePos)
{
	TargetPos = vBonePos;
}
int get_bone_object(ISkeletonPose* m_pSkeleton, const char* Name, const char* ClassName)
{
	if (_strstr(ClassName, "SED")) return 58;
	if (_strstr(Name, "HeavyTurret")) return 52;
	if (_strstr(Name, "IronMan")) return 9;
	if (!_strcmp(ClassName, "Turret")) return m_pSkeleton->GetJointIdByName("bone_head");
	if (_strstr(Name, "Cyborg") || _strstr(Name, "Destroyer")) return 3;
	return m_pSkeleton->GetJointIdByName("Bip01 Head");
}
bool IsAim(const char* Name, const char* ClassName, IEntity* pExp, Vec3& vBonePos)
{
	if (!_strcmp(ClassName, "Mantis") || !_strcmp(ClassName, "Helicopter")) return FALSE;
	if (_strstr(Name, "mech2300") || _strstr(Name, "Helicopter") || _strstr(Name, "Artil")) return FALSE;
	if (_strstr(Name, "TurretAPC")) return FALSE;
	ICharacterInstance* pCharacter = pExp->GetCharacter(0);
	if (pCharacter)
	{
		ISkeletonPose* m_pSkeleton = pCharacter->GetISkeletonPose();
		__int32 szJointId = get_bone_object(m_pSkeleton, Name, ClassName);
		if (szJointId)
		{
			vBonePos = pExp->GetWorldTM() * m_pSkeleton->GetAbsJointByID(szJointId).t;
			return true;
		}
	}
	return false;
}
Vec3    vGetBone(Matrix34 m_pWorld, ISkeletonPose* m_pSkeleton, const char* szJointName)
{
	if (!m_pSkeleton)
		return Vec3{ ZERO };
	__int16 BoneId = m_pSkeleton->GetJointIdByName(szJointName);
	Matrix34 mOut = m_pWorld * Matrix34(m_pSkeleton->GetAbsJointByID(BoneId));
	return mOut.GetTranslation();
}


namespace Cvars {

	static bool autoshot = false;

	static int delay = 0;

	static bool wh = false;

	static bool saim = false;

	static bool arage = false;

	static bool inf = false;

	static bool climb = false;

	static bool podk = false;

	static bool gun = false;
}

ILINE void esp_silhouttes(IEntity* pExp, f32 r, f32 g, f32 b)
{
	if (IEntityRenderProxy * render_proxy = pExp->GetRenderProxy())
	{

		render_proxy->render_flags(ESilhouettesParams::eAllMap);
		render_proxy->hudsilhouettes_params(r, g, b, 0.000f);
	}

}

void shot(IWeapon* mWeapon)
{
	mWeapon->StartFire();
	mWeapon->StopFire();
}
void ClimbExtraHeight(float Value)
{
	DWORD v0 = *(DWORD*)(*(DWORD*)(*(DWORD*)((DWORD)gEnv->pGame + 0x148) + 0x4) + 0x4);
	if (!v0) return;
	*(f32*)(v0 + 0x14) = (Value)+2.5f;
}

void Function()
{
	IActor* m_pActor = nullptr;
	IGameFramework* pFramework = gEnv->pGame->GetIGameFramework();
	if (!pFramework || !pFramework->GetClientActor(&m_pActor) || m_pActor->IsDead()) return;
	IItem* m_pItem = m_pActor->GetCurrentItem();
	TargetPos = ZERO;
	PriorityLimit = 5000.0f;
	IEntityIt* pEntityIt = gEnv->pEntitySystem->GetEntityIterator();
	IWeaponPTR* pWeaponPTR = m_pItem->GetIWeapon()->pWeaponPTR;
	IWeaponParamsOne* pWeaponParamsOne = pWeaponPTR->WeaponParamsOne;
	IWeaponParamsTwo* pWeaponParamsTwo = pWeaponPTR->WeaponParamsTwo;
	IWeaponParamsThree* pWeaponParamsThree = pWeaponPTR->WeaponParamsThree;
	if (!pWeaponParamsOne || !pWeaponParamsTwo || !pWeaponParamsThree) return;
	while (IEntity* pExp = pEntityIt->Next())
	{
		if (auto mWeapon = m_pActor->GetCurrentItem()->GetIWeapon())
		{
			auto mTriggerId = m_pActor->IsTriggerID();
			if (mTriggerId)
			{
				if (IActor * pActor = pFramework->GetIActorSystem()->GetActor(mTriggerId))
				{
					if (IsMyTeam(m_pActor, pActor)) continue;

					if (Cvars::autoshot) shot(mWeapon);
				}
			}
		}
		if (Cvars::wh)
		{
			IEntityClass* pEntClass = pExp->GetClass();
			if (!pEntClass) continue;
			const char* ClassName = pEntClass->GetName();
			const char* Name = pExp->GetName();
			IEntityRenderProxy* pRenderProxy = pExp->GetRenderProxy();
			if (!pRenderProxy) continue;
			uint32 RenderFlags = pRenderProxy->GetFlags();
			if (RenderFlags == 386)
			{
				if (_strstr(Name, "laymore") || _strstr(Name, "apsule") || _strstr(Name, "smartmine") || _strstr(Name, "spidermine") || _strstr(Name, "mine") || _strstr(Name, "heavydroid_rocket"))
				{
					esp_silhouttes(pExp, 100.0f, 0.0f, 0.0f);
				}
			}
			if (IActor * pActor = pFramework->GetIActorSystem()->GetActor(pExp->GetId()))
			{
				if (pActor->IsDead() /*|| pActor == m_pActor*/ || pActor->IsDead() || IsMyTeam(m_pActor, pActor)) continue;
				esp_silhouttes(pExp, 100.0f, 0.0f, 0.0f);
			}
		}
	}
}