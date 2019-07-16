
class SSystemGlobalEnvironment;
class ILog;
class IAISystem;
class IScriptSystem;
class IRenderer;
class IPhysicalWorld;
class IInput;
class ICryPak;
class ITimer;
class IConsole;
class IGame;
class IEntitySystem;
class ISystem;
class I3DEngine;

class IWeaponSystem;
class IItemSystem;

class SCVars;

class IGameFramework;
class IActorSystem;
class IGameRules;

class IEntityClassRegistry;
class IPhysicalEntity;
class IEntityClass;

class IActorIterator;
class IActor;
class IEntity;
class IGameObject;
class IInventory;
class CPlayerRotation;

class IScriptTable;

class IEntityProxy;
class IEntityRenderProxy;
class IEntityScriptProxy;
class IPhysicalProxy;
class ICharacterInstance;

class SEntityObjectXForm;
class IStatObj;

class IItem;
class IWeapon;
class IWeaponPTR;
class IFireMode;

class IEntityScript;

class IGameplayRecorder;
class IMaterialManager;
class CAnimationGraphCVars;

template <typename cData> cData vCall(PVOID Class, int vIndex) { return (*(cData * *)Class)[vIndex]; }

enum class vKey : UINT
{
	KEY_MOUSE_BUTTON_LEFT = 1,
	KEY_MOUSE_BUTTON_RIGHT,
	KEY_CANCEL,
	KEY_MOUSE_BUTTON_MIDDLE,

	KEY_0 = 48,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,

	KEY_A = 65,
	KEY_B,
	KEY_C,
	KEY_D,
	KEY_E,
	KEY_F,
	KEY_G,
	KEY_H,
	KEY_I,
	KEY_J,
	KEY_K,
	KEY_L,
	KEY_M,
	KEY_N,
	KEY_O,
	KEY_P,
	KEY_Q,
	KEY_R,
	KEY_S,
	KEY_T,
	KEY_U,
	KEY_V,
	KEY_W,
	KEY_X,
	KEY_Y,
	KEY_Z,

	KEY_NUMPAD_0 = 96,
	KEY_NUMPAD_1,
	KEY_NUMPAD_2,
	KEY_NUMPAD_3,
	KEY_NUMPAD_4,
	KEY_NUMPAD_5,
	KEY_NUMPAD_6,
	KEY_NUMPAD_7,
	KEY_NUMPAD_8,
	KEY_NUMPAD_9,

	KEY_F1 = 112,
	KEY_F2,
	KEY_F3,
	KEY_F4,
	KEY_F5,
	KEY_F6,
	KEY_F7,
	KEY_F8,
	KEY_F9,
	KEY_F10,
	KEY_F11,
	KEY_F12,

	KEY_SHIFT_LEFT = 160,
	KEY_SHIFT_RIGHT,
	KEY_CTRL_LEFT,
	KEY_CTRL_RIGHT,
	KEY_ALT_LEFT,
	KEY_ALT_RIGHT,

	KEY_ESC = 27,
	KEY_ENTER = 257,
	KEY_TAB,
	KEY_BACKSPACE,

	KEY_SPACE = 0x20,
	KEY_PAGE_UP,
	KEY_PAGE_DOWN,
	KEY_END,
	KEY_HOME,
	KEY_ARROW_LEFT,
	KEY_ARROW_UP,
	KEY_ARROW_RIGHT,
	KEY_ARROW_DOWN,

	KEY_PAUSE = 0x13,
	KEY_CAPS_LOCK,

	KEY_NUM_LOCK = 0x90,
	KEY_SCROLL_LOCK,

	KEY_PRINT_SCREEN = 0x2C,
	KEY_INSERT = 20,
	KEY_DELETE = 0x2E
};

/*
#define gEnv (*(SSystemGlobalEnvironment**)0x1937640)
#define gRender (*(IRenderer**)0x1B5B61C)
#define pGameCVars (*(SCVars**)0x01B6E338)
*/
#define gEnv (*(SSystemGlobalEnvironment**)0x190E04C) // ai_comp
#define gRender (*(IRenderer**)0x1BA1E0C) // d3dtexture
#define pGameCVars (*(SCVars**)0x11BF6BD4) //issame
#define Framework (*(IGameFramework**)0x1BF712C) // failed to create

inline int _strlen(const char* str)
{
	int n = 0;
	while (*str++) n++;
	return n;
}
inline int _strcmp(const char* str1, const char* str2)
{
	int result = 0;
	int str1_len = _strlen(str1);
	int str2_len = _strlen(str2);
	while (*str1 || *str2)
	{
		if (*str1 > * str2)
		{
			result = 1;
			break;
		}
		else if (*str1 < *str2)
		{
			result = -1;
			break;
		}
		*str1++;
		*str2++;
	}
	if (!result && (str1_len < str2_len)) result = 1;
	else if (!result && (str1_len > str2_len)) result = -1;
	return result;
}
inline bool _strstr(const char* str, const char* pattern)
{
	char* cp = (char*)str;
	char* s1, * s2;
	if (*pattern)
	{
		while (*cp)
		{
			s1 = cp;
			s2 = (char*)pattern;
			while (*s1 && *s2 && !(*s1 - *s2)) s1++, s2++;
			if (!*s2) return true;
			cp++;
		}
	}
	return false;
}

ILINE void _mathVec4Transform(f32 out[4], const f32 m[16], const f32 in[4]) {
#define M(row, col) m[col * 4 + row]
	out[0] = M(0, 0) * in[0] + M(0, 1) * in[1] + M(0, 2) * in[2] + M(0, 3) * in[3];
	out[1] = M(1, 0) * in[0] + M(1, 1) * in[1] + M(1, 2) * in[2] + M(1, 3) * in[3];
	out[2] = M(2, 0) * in[0] + M(2, 1) * in[1] + M(2, 2) * in[2] + M(2, 3) * in[3];
	out[3] = M(3, 0) * in[0] + M(3, 1) * in[1] + M(3, 2) * in[2] + M(3, 3) * in[3];
#undef M
}
ILINE f32 _mathVec3Project(Vec3* pvWin, const Vec3* pvObj, const int32 pViewport[4], const Matrix44* pProjection, const Matrix44* pView, const Matrix44* pWorld) {
	Vec4 in, out;
	in.x = pvObj->x;
	in.y = pvObj->y;
	in.z = pvObj->z;
	in.w = 1.0f;
	_mathVec4Transform((f32*)& out, (f32*)pWorld, (f32*)& in);
	_mathVec4Transform((f32*)& in, (f32*)pView, (f32*)& out);
	_mathVec4Transform((f32*)& out, (f32*)pProjection, (f32*)& in);
	if (out.w == 0.0f) return 0.f;
	out.x /= out.w;
	out.y /= out.w;
	out.z /= out.w;
	pvWin->x = pViewport[0] + (1 + out.x) * pViewport[2] / 2;
	pvWin->y = pViewport[1] + (1 - out.y) * pViewport[3] / 2;
	float fViewportMinZ = 0, fViewportMaxZ = 1.0f;
	pvWin->z = fViewportMinZ + out.z * (fViewportMaxZ - fViewportMinZ);
	return out.w;
}

class ClientDevice
{
public:
	DWORD* pDevice;
};
class IRenderer
{
public:
	FLOAT GetHypotenuseFromCenter(Vec3 Point) {
		Vec3 Screen;
		FLOAT ScreenCenterX = (Width / 2.0f);
		FLOAT ScreenCenterY = (Height / 2.0f);
		if (ProjectToScreen(Point, Screen) == FALSE)return 9999.0f;
		FLOAT PositionX = Screen.x > ScreenCenterX ? Screen.x - ScreenCenterX : ScreenCenterX - Screen.x;
		FLOAT PositionY = Screen.y > ScreenCenterY ? Screen.y - ScreenCenterY : ScreenCenterY - Screen.y;
		return (FLOAT)_sqrt(PositionX * PositionX + PositionY * PositionY);
	}
	void DrawAABB(AABB& aabb) { return vCall<void(__thiscall*)(PVOID, AABB&)>(this, 0x404 / 4)(this, aabb); }
	bool ProjectToScreen(int32* vp, FLOAT ptx, FLOAT pty, FLOAT ptz, FLOAT* sx, FLOAT* sy, FLOAT* sz) {
		Vec3 vOut, vIn;
		vIn.x = ptx;
		vIn.y = pty;
		vIn.z = ptz;
		int32 v[4] = { vp[0], vp[1], vp[2], vp[3] };
		Matrix44 mIdent;
		mIdent.SetIdentity();
		Matrix44 View = m_uBaseProjection;
		Matrix44 Projection = m_uBaseMatrix;
		if (_mathVec3Project(&vOut, &vIn, v, &Projection, &View, &mIdent)) {
			*sx = vOut.x * 100.0f / (f32)v[2];
			*sy = vOut.y * 100.0f / (f32)v[3];
			*sz = vOut.z;
			return true;
		}
		return false;
	}
	bool ProjectToScreen(Vec3 in, Vec3& out) {
		int32 v[4] = { 0, 0, Width, Height };
		ProjectToScreen(v, in.x, in.y, in.z, &out.x, &out.y, &out.z);
		if (out.z < 0.0f || out.z > 1.0f) return false;
		out.x = out.x * (v[2] / 100.0f);
		out.y = out.y * (v[3] / 100.0f);
		return true;
	}
	char pad_0x0000[0x50]; //0x0000
	Matrix44 m_uBaseProjection; //0x0050
	char pad_0x0090[0xC0]; //0x0090
	Matrix44 m_uBaseMatrix; //0x0150
	char pad_0x0190[0x40]; //0x0190
	Matrix44 CameraMatrix; //0x01D0
	char pad_0x0210[0xE34]; //0x0210
	Vec3 vPos; //0x1044
	char pad_0x1050[0x5FC8]; //0x1050
	__int32 Width; //0x7018
	__int32 Height; //0x701C
	char pad_0x7020[0xD34]; //0x7020
	DWORD hWnd; //0x7D54
	char pad_0x7D58[0x118]; //0x7D58
	ClientDevice* GetDevice; //0x7E70
}; //Size=0x7E74

class ISystem
{
public:
	int IsSerializingFile()
	{
		return *(uint32*)((DWORD)this + 0x724);
	}
};

class IXmlNode
{
protected:
	int m_nRefCount;

public:
	template<typename T> bool GetAttrImpl(const char* name, T& valueOut);//27
	const char* getTag();//7
};

class XmlNodeRef
{
private:
	IXmlNode* p;
public:
	XmlNodeRef() : p(NULL) {}
	XmlNodeRef(IXmlNode* p_);
	XmlNodeRef(const XmlNodeRef& p_);

	~XmlNodeRef();

	operator IXmlNode* () const { return p; }

	IXmlNode& operator*() const { return *p; }
	IXmlNode* operator->(void) const { return p; }

	XmlNodeRef& operator=(IXmlNode* newp);
	XmlNodeRef& operator=(const XmlNodeRef& newp);

#if !defined(RESOURCE_COMPILER)
	template<typename Sizer>
	void GetMemoryUsage(Sizer* pSizer) const
	{
		pSizer->AddObject(p);
	}
#endif
};

class SmartScriptTable
{
public:
	SmartScriptTable() : p(NULL) {};
	IScriptTable* operator->() const { return p; }
	IScriptTable* operator*() const { return p; }
	operator const IScriptTable* () const { return p; }
	operator IScriptTable* () const { return p; }
	operator bool() const { return (p != NULL); }

	bool operator!() const { return p == NULL; };
	bool operator==(const IScriptTable* p2) const { return p == p2; };
	bool operator==(IScriptTable* p2) const { return p == p2; };
	bool operator!=(const IScriptTable* p2) const { return p != p2; };
	bool operator!=(IScriptTable* p2) const { return p != p2; };
	bool operator<(const IScriptTable* p2) const { return p < p2; };
	bool operator>(const IScriptTable* p2) const { return p > p2; };
	IScriptTable* GetPtr() const { return p; }
private:
	IScriptTable* p;
};


struct ray_hit
{
	float dist;
	IPhysicalEntity* pCollider;
	int ipart;
	int partid;
	short surface_idx;
	short idmatOrg;
	int foreignIdx;
	int iNode;
	Vec3 pt;
	Vec3 n;
	int bTerrain;
	int iPrim;
	ray_hit* next;
};
struct ray_hit_cached : ray_hit
{
	ray_hit_cached() { pCollider = 0; ipart = 0; }
	ray_hit_cached(const ray_hit& hit)
	{
		pCollider = hit.pCollider; ipart = hit.ipart;
		iNode = hit.iNode;
	}
	ray_hit_cached& operator=(const ray_hit& hit)
	{
		pCollider = hit.pCollider; ipart = hit.ipart;
		iNode = hit.iNode; return *this;
	}
};
struct SRWIParams
{
	PVOID pForeignData; //0x0000 
	__int32 iForeignData; //0x0004 
	char pad_0x0008[0x4]; //0x0008 
	Vec3 org; //0x000C 
	Vec3 dir; //0x0018 
	__int32 objtypes; //0x0024 
	__int32 flags; //0x0028 
	ray_hit* hits; //0x002C 
	__int32 nMaxHits; //0x0030 
	ray_hit_cached* phitLast; //0x0034 
	__int32 nSkipEnts; //0x0038 
	IPhysicalEntity** pSkipEnts; //0x003C 
}; //Size=0x0040

class ICVar {
public:
	const char* GetName() { return m_szName; }
	int GetType() { return vCall<int(__thiscall*)(PVOID)>(this, 35)(this); }
	template <class cData> void SetValue(cData Value)
	{
		vCall<void(__thiscall*)(PVOID, cData)>(this, 7)(this, Value);
	}
public:
	char pad_0x0000[0x4]; //0x0000
	char* m_szName; //0x0004 
};

class IEntityClass
{
public:
	const char* GetName() { return m_sName; }
private:
	char pad_0x0000[0x8]; //0x0000
	char* m_sName; //0x0008 
	char pad_0x000C[0x4]; //0x000C
	char* m_sScriptFile; //0x0010 
}; //Size=0x0014

class IInventory
{
public:
	void SetAmmoCount(IEntityClass* pAmmoType, int count) { return vCall<void(__thiscall*)(PVOID, IEntityClass*, int)>(this, 58)(this, pAmmoType, count); }
	int GetAmmoCount(IEntityClass* pAmmoType) { return vCall<int(__thiscall*)(PVOID, IEntityClass*)>(this, 59)(this, pAmmoType); }
	void SetAmmoCapacity(IEntityClass* pAmmoType, int max) { return vCall<void(__thiscall*)(PVOID, IEntityClass*, int)>(this, 60)(this, pAmmoType, max); }
	int GetAmmoCapacity(IEntityClass* pAmmoType) { return vCall<int(__thiscall*)(PVOID, IEntityClass*)>(this, 61)(this, pAmmoType); }
	void ResetAmmo() { return vCall<void(__thiscall*)(PVOID)>(this, 62)(this); }
	EntityId GetCurrentItemId() { return vCall <EntityId(__thiscall*)(PVOID)>(this, 184 / 4)(this); }
};

class ISkeletonPose {
public:
	QuatT GetAbsJointByID(__int32 szJointId)
	{
		return *(QuatT*)(*(DWORD*)((DWORD)this + 0x114) + sizeof(QuatT) * szJointId);
	}
	__int32 GetJointCount()
	{
		if (auto v2 = *(DWORD*)((DWORD)this + 0x104)) return *(DWORD*)(v2 + 0x14);
		return -1;
	}
	UINT GetJointIdByName(const char* strJointName) { return (UINT)vCall<__int32(__thiscall*)(PVOID, const char*)>(this, 3)(this, strJointName); }
};
class ICharacterInstance {
public:
	ISkeletonPose* GetISkeletonPose() { return &pSkeletonPose; }
private:
	char pad_0x0000[0x748]; //0x0000
	ISkeletonPose pSkeletonPose; //0x0748 
};

struct coord_block
{
	Vec3 pos;
	quaternionf q;
};
struct pe_params
{
	int type;
};
struct pe_player_dynamics : pe_params
{
	enum entype { type_id = ePE_player_dynamics };
	pe_player_dynamics()
	{
		type = type_id;
	}

	float            kInertia;                            //!< inertia koefficient, the more it is, the less inertia is; 0 means no inertia
	float            kInertiaAccel;                       //!< inertia on acceleration
	float            kAirControl;                         //!< air control koefficient 0..1, 1 - special value (total control of movement)
	float            kAirResistance;                      //!< standard air resistance
	Vec3             gravity;                             //!< gravity vector
	float            nodSpeed;                            //!< vertical camera shake speed after landings
	int              bSwimming;                           //!< whether entity is swimming (is not bound to ground plane)
	float            mass;                                //!< mass (in kg)
	int              surface_idx;                         //!< surface identifier for collisions
	float            minSlideAngle;                       //!< if surface slope is more than this angle, player starts sliding (angle is in radians)
	float            maxClimbAngle;                       //!< player cannot climb surface which slope is steeper than this angle
	float            maxJumpAngle;                        //!< player is not allowed to jump towards ground if this angle is exceeded
	float            minFallAngle;                        //!< player starts falling when slope is steeper than this
	float            maxVelGround;                        //!< player cannot stand of surfaces that are moving faster than this
	float            timeImpulseRecover;                  //!< forcefully turns on inertia for that duration after receiving an impulse
	int              collTypes;                           //!< entity types to check collisions against
	IPhysicalEntity* pLivingEntToIgnore;                  //!< ignore collisions with this *living entity* (doesn't work with other entity types)
	int              bNetwork;                            //!< uses extended history information (obsolete)
	int              bActive;                             //!< 0 disables all simulation for the character, apart from moving along the requested velocity
	int              iRequestedTime;                      //!< requests that the player rolls back to that time and re-executes pending actions during the next step
	int              bReleaseGroundColliderWhenNotActive; //!< when not 0, if the living entity is not active, the ground collider, if any, will be explicitly released during the simulation step.
};
struct pe_simulation_params : pe_params
{
	enum entype { type_id = ePE_simulation_params };
	pe_simulation_params()
	{
		type = type_id;
	}

	int   iSimClass;
	float maxTimeStep;       //!< Maximum time step that entity can accept (larger steps will be split).
	float minEnergy;         //!< Minimun of kinetic energy below which entity falls asleep (divided by mass).
	float damping;           //!< Damped velocity = oridinal velocity * (1 - damping*time interval).
	Vec3  gravity;           //!< Per-entity gravity (note that if there are any phys areas with gravity, they will override it unless pef_ignore_areas is set.
	float dampingFreefall;   //!< Damping and gravity used when there are no collisions.
	Vec3  gravityFreefall;   //!< NOTE: if left unused, gravity value will be substituted (if provided).
	float maxRotVel;         //!< Rotational velocity is clamped to this value.
	float mass;              //!< Either mass of density should be set; mass = density*volume.
	float density;
	int   maxLoggedCollisions; //!< Maximum EventPhysCollisions reported per frame (only supported by rigid bodies/ragdolls/vehicles).
	int   disablePreCG;        //!< Disables Pre-CG solver for the group this body is in (recommended for balls).
	float maxFriction;         //!< Sets upper friction limit for this object and all objects it's currently in contact with.
	int   collTypes;           //!< Collision types (a combination of ent_xxx flags).
};
struct pe_params_particle : pe_params
{
	enum entype { type_id = ePE_params_particle };
	pe_params_particle()
	{
		type = type_id;
	}

	unsigned int     flags; //!< see entity flags
	float            mass;
	float            size;             //!< pseudo-radius
	float            thickness;        //!< thickness when lying on a surface (if left unused, size will be used)
	Vec3             heading;          //!< direction of movement
	float            velocity;         //!< velocity along "heading"
	float            kAirResistance;   //!< air resistance koefficient, F = kv
	float            kWaterResistance; //!< same for water
	float            accThrust;        //!< acceleration along direction of movement
	float            accLift;          //!< acceleration that lifts particle with the current speed
	int              surface_idx;
	Vec3             wspin;             //!< angular velocity
	Vec3             gravity;           //!< stores this gravity and uses it if the current area's gravity is equal to the global gravity
	Vec3             waterGravity;      //!< gravity when underwater
	Vec3             normal;            //!< aligns this direction with the surface normal when sliding
	Vec3             rollAxis;          //!< aligns this directon with the roll axis when rolling (0,0,0 to disable alignment)
	quaternionf      q0;                //!< initial orientation (zero means x along direction of movement, z up)
	float            minBounceVel;      //!< velocity threshold for bouncing->sliding switch
	float            minVel;            //!< sleep speed threshold
	IPhysicalEntity* pColliderToIgnore; //!< physical entity to ignore during collisions
	int              iPierceability;    //!< pierceability for ray tests; pierceble hits slow the particle down, but don't stop it
	int              collTypes;         //!< 'objtype' passed to RayWorldntersection
	int              areaCheckPeriod;   //!< how often (in frames) world area checks are made
	int              dontPlayHitEffect; //!< prevent playing of material FX from now on
};


class IPhysicalEntity
{
public:
	int GetParams(pe_params* _params)
	{
		return vCall<int(__thiscall*)(PVOID, pe_params*)>(this, 6)(this, _params);
	}
	char _0x0000[4]; //0x0000
	Vec3 m_BBox[2]; //0x0004
	IEntity* m_pEntity; //0x001C 
	char pad_0x0020[0x34]; //0x0020
	IPhysicalWorld* m_pWorld; //0x0054 
	char pad_0x0058[0x18]; //0x0058
	Vec3 m_pos; //0x0070
	quaternionf m_qrot; //0x007C
	coord_block* m_pNewCoords; //0x008C
};
class IPhysicalProxy
{
public:
	IPhysicalEntity* GetPhysicalEntity() { return m_pPhysicalEntity; }
private:
	char pad_0x0000[0xC]; //0x0000
	IPhysicalEntity* m_pPhysicalEntity; //0x000C
};

class IEntityObject
{
public:
	SEntityObjectXForm* m_pXForm; //0x0000 
	Matrix34 m_worldTM; //0x0004 
	Matrix34 m_worldPrevTM; //0x0034 
	IEntityObject* pParent; //0x0064 
	IStatObj* pStatObj; //0x0068 
	ICharacterInstance* pCharacter; //0x006C
};
class IRenderNode
{
public:
	void SetViewDistRatio(int nViewDistRatio)
	{
		return vCall<void(__thiscall*)(PVOID, int)>(this, 58)(this, nViewDistRatio);
	}

	void SetViewDistUnlimited()
	{
		SetViewDistRatio(255);
	}
};
class IEntityRenderProxy
{
public:
	enum EFlags
	{
		FLAG_CUSTOM_POST_EFFECT = BIT(0), // Has custom post effect ID stored in custom data
		FLAG_BBOX_VALID_LOCAL = BIT(1),
		FLAG_BBOX_FORCED = BIT(2),
		FLAG_BBOX_INVALID = BIT(3),
		FLAG_HIDDEN = BIT(4), // If render proxy is hidden.
		FLAG_UPDATE = BIT(5),  // If render proxy needs to be updated.
		FLAG_NOW_VISIBLE = BIT(6),  // If render proxy currently visible.
		FLAG_REGISTERED_IN_3DENGINE = BIT(7),  // If render proxy have been registered in 3d engine.
		FLAG_POST_INIT = BIT(8),  // If render proxy have received Post init event.
		FLAG_HAS_LIGHTS = BIT(9),  // If render proxy has some lights.
		FLAG_GEOMETRY_MODIFIED = BIT(10), // Geometry for this render slot was modified at runtime.
		FLAG_HAS_CHILDRENDERNODES = BIT(11), // If render proxy contains child render nodes
		FLAG_HAS_PARTICLES = BIT(12), // If render proxy contains particle emitters
		FLAG_SHADOW_DISSOLVE = BIT(13), // If render proxy requires dissolving shadows
		FLAG_FADE_CLOAK_BY_DISTANCE = BIT(14), // If render proxy requires fading cloak by distance
		FLAG_IGNORE_HUD_INTERFERENCE_FILTER = BIT(15), // HUD render proxy ignores hud interference filter post effect settings
		FLAG_IGNORE_HEAT_VALUE = BIT(16), // Don't appear hot in nano vision
		FLAG_POST_3D_RENDER = BIT(17), // Render proxy in post 3D pass
		FLAG_IGNORE_CLOAK_REFRACTION_COLOR = BIT(18), // Will ignore cloak refraction color
		FLAG_HUD_REQUIRE_DEPTHTEST = BIT(19), // If 3D HUD Object requires to be rendered at correct depth (i.e. behind weapon)
		FLAG_CLOAK_INTERFERENCE = BIT(20), // When set the cloak will use the cloak interference parameters
		FLAG_CLOAK_HIGHLIGHTS = BIT(21), // When set the cloak will use the cloak highlight parameters
		FLAG_HUD_DISABLEBLOOM = BIT(22), // Allows 3d hud object to disable bloom (Required to avoid overglow and cutoff with alien hud ghosted planes)
		FLAG_ANIMATE_OFFSCREEN_SHADOW = BIT(23), // Update the animation if object drawn in the shadow pass
		FLAG_DISABLE_MOTIONBLUR = BIT(24), // Disable motion blur
		FLAG_EXECUTE_AS_JOB_FLAG = BIT(25), // set if this CRenderProxy can be executed as a Job from the 3DEngine
		FLAG_RECOMPUTE_EXECUTE_AS_JOB_FLAG = BIT(26), // set if the slots changed, to recheck if this renderproxy can execute as a job
	};

	IRenderNode* GetRenderNode() { return (IRenderNode*)((DWORD)this + 0x4); }

	ILINE void   SetFlags(uint32 flags) { m_nFlags = flags; };
	ILINE uint32 GetFlags() { return *(uint32*)((DWORD)this + 0x6C)/*m_nFlags*/; };
	ILINE void   AddFlags(uint32 flagsToAdd) { SetFlags(m_nFlags | flagsToAdd); };
	ILINE void   ClearFlags(uint32 flagsToClear) { SetFlags(m_nFlags & ~flagsToClear); };
	ILINE bool   CheckFlags(uint32 flagsToCheck) const { return (m_nFlags & flagsToCheck) == flagsToCheck; };

	void GetWorldBounds(AABB& bbox)
	{
		return vCall<void(__thiscall*)(PVOID, AABB&)>(this, 13)(this, bbox);
	}
	void SetHUDSilhouettesParams(float r, float g, float b, float a = 255)
	{
		AABB aabb;
		GetWorldBounds(aabb);
		gRender->DrawAABB(aabb);
		m_nHUDSilhouettesParams = (uint32)(int_round(a * 255.0f) << 24) | (int_round(r * 255.0f) << 16) | (int_round(g * 255.0f) << 8) | (int_round(b * 255.0f));
		m_dwRndFlags |= ESilhouettesParams::eAllMap;
	}
	void SetHUDSilhouettesParams(uint32 c)
	{
		AABB aabb;
		GetWorldBounds(aabb);
		gRender->DrawAABB(aabb);
		m_nHUDSilhouettesParams = c;
		m_dwRndFlags |= ESilhouettesParams::eAllMap;
	}

	void render_flags(int32 flag)
	{
		*(int32*)((int32)this + 0x14) = flag;
	}

	void hudsilhouettes_params(f32 r, f32 g, f32 b, f32 a)
	{
		return vCall<void(__thiscall*)(PVOID, f32, f32, f32, f32)>(this, 176 / 4)(this, r, g, b, a);
	}

	char pad_0x0000[0x10]; //0x0000
	float m_fWSMaxViewDist; //0x0010
	uint32 m_dwRndFlags; //0x0014
	char pad_0x0018[0x8]; //0x0018
	uint32 m_nInternalFlags; //0x0020
	char pad_0x0024[0x8]; //0x0024
	IEntity* m_pEntity; //0x002C
	char pad_0x0030[0xC]; //0x0030
	AABB m_localBBox; //0x003C
	char pad_0x0054[0x10]; //0x0054
	EntityId m_entityId; //0x0064
	uint32 m_nEntityFlags; //0x0068
	uint32 m_nFlags; //0x006C
	char pad_0x0070[0x8]; //0x0070
	uint32 m_nHUDSilhouettesParams; //0x0078 //RGBA
	char pad_0x007C[0x8]; //0x007C
	bool m_nCloakBlendTimeScale; //0x0084
	bool m_nOpacity; //0x0085
	bool m_nCloakColorChannel; //0x0086
	bool m_nCustomData; //0x0087
	AABB m_WSBBox; //0x0088
};

class IEntityScript
{
public:
	char pad_0x0000[0x4]; //0x0000
	IScriptSystem* m_pScriptSystem; //0x0004 
	char pad_0x0008[0x8]; //0x0008
	char* m_sTableName; //0x0010 
	char pad_0x0014[0x4]; //0x0014
	char* m_sScriptFilename; //0x0018 
	char pad_0x001C[0x28]; //0x001C

}; //Size=0x0044

class IScriptTable
{
public:
	void SetToNull(const char* sKey) { ((HRESULT(WINAPI*)(PVOID, const char*))0x5F8570)(this, sKey); }
};

class IEntityScriptProxy
{
public:
	IScriptTable* GetScriptTable()
	{
		return m_pThis;
	}
private:
	char pad_0x0000[0x4]; //0x0000
	IEntity* m_pEntity; //0x0004 
	IEntityScript* m_pScript; //0x0008 
	IScriptTable* m_pThis; //0x000C 
};
class IEntity
{
public:
	bool IsGarbage()
	{
		return (*(DWORD*)((DWORD)this + 8) >> 5) & 1;
	}
	IEntityProxy* GetProxy(EEntityProxy proxy)
	{
		if (*(DWORD*)((DWORD)this + 0xA0) > 0 && *(DWORD*)((DWORD)this + 0xA0) <= 5)
			return *(IEntityProxy * *)(*(DWORD*)((DWORD)this + 0x9C) + (DWORD)proxy * 4);
		return NULL;
	}
	EntityId GetId() { return m_nID; }
	IEntityClass* GetClass() { return m_pClass; }
	const char* GetName() { return *(char**)((DWORD)this + 0xB8); }
	wchar_t* GetNameW() { return *(wchar_t**)((DWORD)this + 0xB8); }
	Matrix34 GetWorldTM() { return m_worldTM; }
	DWORD GetFlags() { return *(DWORD*)((DWORD)this + 0x4); }

	void SetName(const char* sName) { return vCall<void(__thiscall*)(PVOID, const char*)>(this, 11)(this, sName); }

	Vec3 GetWorldPos() { return m_worldTM.GetTranslation(); }

	void  SetPos(const Vec3& vPos, int nWhyFlags = 0xA)
	{
		m_vPos = vPos;
		InvalidateTM(nWhyFlags | ENTITY_XFORM_POS);
	}
	void  SetRotation(const Quat& qRotation, int nWhyFlags = 0)
	{
		m_qRotation = qRotation;
		InvalidateTM(nWhyFlags | ENTITY_XFORM_ROT);
	}
	void  SetScale(const Vec3& vScale, int nWhyFlags = 0)
	{
		m_vScale = vScale;
		InvalidateTM(nWhyFlags | ENTITY_XFORM_SCL);
	}

	Vec3 GetPos() { return m_vPos; }
	Quat& GetRotation() { return m_qRotation; }
	Vec3& GetScale() { return m_vScale; }

	void InvalidateTM(int nWhyFlags) { vCall<void(__thiscall*)(PVOID, int)>(this, 95)(this, nWhyFlags); }
	void GetWorldBounds(AABB& BoundBox) { vCall<void(__thiscall*)(PVOID, AABB&)>(this, 25)(this, BoundBox); }
	IEntityRenderProxy* GetRenderProxy() { return (IEntityRenderProxy*)GetProxy(ENTITY_PROXY_RENDER); }
	IPhysicalProxy* GetPhysicalProxy() { return (IPhysicalProxy*)GetProxy(ENTITY_PROXY_PHYSICS); }
	ICharacterInstance* GetCharacter(int nSlot = 0) { return vCall<ICharacterInstance * (__thiscall*)(PVOID, INT)>(this, 0x148 / 4)(this, nSlot); }
	IPhysicalEntity* GetPhysics()
	{
		if (IPhysicalProxy * m_pPhysicalProxy = GetPhysicalProxy()) return m_pPhysicalProxy->GetPhysicalEntity();
		return NULL;
	}
	IScriptTable* GetScriptTable()
	{
		IEntityScriptProxy* pScriptProxy = (IEntityScriptProxy*)GetProxy(ENTITY_PROXY_SCRIPT);
		if (pScriptProxy) return pScriptProxy->GetScriptTable();
		return NULL;
	}
private:
	char pad_0x0000[0x4]; //0x0000
	uint32 m_flags; //0x0004 
	char pad_0x0008[0xC]; //0x0008
	EntityId m_nID; //0x0014 
	char pad_0x0018[0x10]; //0x0018
	IEntityClass* m_pClass; //0x0028 
	char pad_0x002C[0x4]; //0x002C
	Matrix34 m_worldTM; //0x0030 
	Vec3 m_vPos; //0x0060 
	Vec3 m_vScale; //0x006C 
	Quat m_qRotation; //0x0078 
	char pad_0x0088[0x30]; //0x0088
	char* m_szName; //0x00B8
};

class IWeaponParamsOne
{
public:
	char _0x0000[8];
	__int32 Damage;
};

class IWeaponParamsThree
{
public:
	char pad_0x0000[0x4]; //0x0000
	bool UnlimitedAmmo; //0x0004
	char pad_0x0005[0x3]; //0x0005
	__int32 Ammo; //0x0008
};
class IWeaponParamsTwo
{
public:
	float fSwayScaleX; //0x0000
	float fSwayScaleY; //0x0004
	float fZoomSwayScaleX; //0x0008
	float fZoomSwayScaleY; //0x000C
	char pad_0x0010[0x50]; //0x0010
	DWORD m_mapweapon; //0x0060
};

class IWeaponPTR
{
public:
	char _0x0000[36];
	IWeaponParamsOne* WeaponParamsOne; //0x0024 
	char _0x0028[8];
	IWeaponParamsTwo* WeaponParamsTwo; //0x0030 
	IWeaponParamsThree* WeaponParamsThree; //0x0034

	int GetAmmoCount() { return WeaponParamsThree->Ammo; }
	void UnlimAmmo(bool status) { WeaponParamsThree->UnlimitedAmmo = status; }
	bool IsUnlimAmmo() { return WeaponParamsThree->UnlimitedAmmo; }
	bool IsFiring()
	{
		DWORD v12 = *(DWORD*)((DWORD)this + 0x70);
		return (v12 == 3 || v12 == 6 || v12 == 12 || v12 == 13 || v12 == 14 || v12 == 15 || v12 == 16 || v12 == 48 || v12 == 49);
	}

	void StartFire()
	{
		DWORD pStartFire = *(DWORD*)((DWORD)this + 0xF8);
		*(DWORD*)((DWORD)this + 0xF4) = pStartFire;
	}
	void StopFire()
	{
		DWORD pStopFire = *(DWORD*)((DWORD)this + 0xF0);
		*(DWORD*)((DWORD)this + 0xF4) = pStopFire;
	}
};

class IFireMode
{
public:
	char pad_0x0000[0x4]; //0x0000
	IWeaponPTR* pWeaponPtr; //0x0004 

	void StartFire() { vCall<void(_fastcall*)(PVOID)>(this, 34)(this); }
	void StopFire() { vCall<void(_fastcall*)(PVOID)>(this, 35)(this); }
};

class IWeapon
{
public:
	char _0x0000[4];
	IWeaponPTR* pWeaponPTR;
	char pad_0x0008[0x20]; //0x0008
	IFireMode* m_fm; //0x0028 
	IWeaponParamsOne* WeaponParamsOne; //0x0024 
	char _0x0028[8];
	IWeaponParamsTwo* WeaponParamsTwo; //0x0030 
	IWeaponParamsThree* WeaponParamsThree; //0x0034
	void UnlimAmmo(bool status) { WeaponParamsThree->UnlimitedAmmo = status; }


	void FireAnimate(int Status)
	{
		*(DWORD*)((DWORD)this + 0x70) = Status;
	}
	void StartFireTest()
	{
		if (DWORD pStartFire = *(DWORD*)((DWORD)pWeaponPTR + 0xF4)) pWeaponPTR->StartFire();
		else m_fm->StartFire();
	}

	void ResetFiringPos()
	{
		if (auto v0 = *(DWORD*)((DWORD)this + 0xC))* (Vec3*)(v0 + 0x10) = Vec3(0, 0, 0);
	}
	void SetFiringPos(Vec3 vPos)
	{
		if (auto v0 = *(DWORD*)((DWORD)this + 0xC))* (Vec3*)(v0 + 0x10) = vPos;
	}

	VOID StartFire()
	{
		using oStartFire = VOID(__thiscall*)(PVOID);
		return _callFunction<oStartFire>(this, 44)(this);
	}

	VOID StopFire()
	{
		using oStopFire = VOID(__thiscall*)(PVOID);
		return _callFunction<oStopFire>(this, 48)(this);
	}

	template <typename T>
	T _callFunction(VOID* Base, INT index) { return (*(T * *)Base)[index / 4]; }
};

class IItem
{
public:
	IEntity* GetEntity() { return m_pEntity; }
	EntityId GetEntityId() { return entid; }
	IWeapon* GetIWeapon() { return (IWeapon*)((DWORD)this + 0x14); }
	void PickUp(EntityId picker, bool sound, bool select = true, bool keepHistory = true, const char* setup = NULL)
	{
		return vCall<void(__thiscall*)(PVOID, EntityId, bool, bool, bool, const char*)>(this, 42)(this, picker, sound, select, keepHistory, setup);
	}
private:
	char _0x0000[8];
	IEntity* m_pEntity; //0x0008 
	char _0x000C[4];
	EntityId entid; //0x0010 
	IWeapon m_pWeapon; //0x0014 
};

class CPlayerRotation
{
public:
	char pad_0x0000[0x8]; //0x0000
	f32 Growth; //0x0008 
	char pad_0x000C[0x74]; //0x000C
	Quat m_viewQuat; //0x0080 
	Quat m_viewQuatFinal; //0x0090 
	Quat m_baseQuat; //0x00A0 
	char pad_0x00B0[0x1]; //0x00B0
	bool Recoil; //0x00B1 
	char pad_0x00B2[0xE]; //0x00B2
	Vec3 m_pos; //0x00C0

	void SetViewRotation(const Quat& rotation)
	{
		m_baseQuat = rotation;
		m_viewQuat = rotation;
		m_viewQuatFinal = rotation;
	}
}; //Size=0x00CC
class IActor
{
public:
	int GetHealth() { return vCall<int(__thiscall*)(PVOID)>(this, 27)(this); }
	bool IsDead() { return GetHealth() <= 0; }
	void SetViewRotation(const Quat& rotation) { return vCall<void(__thiscall*)(PVOID, const Quat&)>(this, 50)(this, rotation); }
	IItem* GetCurrentItem(bool includeVehicle = false) { return vCall<IItem * (__thiscall*)(PVOID, bool)>(this, 56)(this, includeVehicle); }
	VOID slideDistanceMulting(FLOAT Value) { *(FLOAT*)((DWORD)this + 0x0C8C) = Value; }
	IInventory* GetInventory() { return vCall<IInventory * (__thiscall*)(PVOID)>(this, 232 / 4)(this); }
	auto IsTriggerID()
	{
		auto u = *(int*)((DWORD)this + 0xC);
		u = *(int*)((DWORD)u + 0x40);
		u = *(int*)((DWORD)u + 0x18);
		return (int) * (DWORD*)((DWORD)u + 0x110);
	}
	char pad_0x0000[0x8]; //0x0000
	IEntity* m_pEntity; //0x0008
	IGameObject* m_pGameObject; //0x000C
	EntityId m_pID; //0x0010
	char pad_0x0014[0x8]; //0x0014
	IInventory* m_pInventory; //0x001C
	char pad_0x0020[0x20]; //0x0020
	DWORD64 Health; //0x0040
	char pad_0x0048[0x350]; //0x0048
	__int32 TeamId; //0x0398
	char pad_0x039C[0x58]; //0x039C
	__int32 ClassId; //0x03F4
	char pad_0x03F8[0x5C0]; //0x03F8
	CPlayerRotation* m_pPlayerRotation; //0x09B8
	char pad_0x09BC[0x1C]; //0x09BC
	Vec3 m_pos; //0x09D8
	char pad_0x09E4[0x11C]; //0x09E4
	float viewPivot; //0x0B00
	char pad_0x0B04[0x8]; //0x0B04
	float viewDistance; //0x0B0C
	float viewHeightOffset; //0x0B10
	float viewFoVScale; //0x0B14
	float viewSensitivity; //0x0B18
	float viewLimitDir; //0x0B1C
	char pad_0x0B20[0x8]; //0x0B20
	float viewLimitYaw; //0x0B28
	float viewLimitPitch; //0x0B2C
	char pad_0x0B30[0x8]; //0x0B30
	float weaponBobbingMultiplier; //0x0B38
	float weaponInertiaMultiplier; //0x0B3C
	float speedMultiplier; //0x0B40
	float useAdditiveAiming; //0x0B44
	float strafeMultiplier; //0x0B48
	char pad_0x0B4C[0x4]; //0x0B4C
	float backwardMultiplier; //0x0B50
	float crouchMultiplier; //0x0B54
	float claymore_detector_radius; //0x0B58
	float claymore_activation_delay_mul; //0x0B5C
	char pad_0x0B60[0x4]; //0x0B60
	float hp_regeneration_hps; //0x0B64
	float hp_regeneration_timeout; //0x0B68
	float armor_regen_hps; //0x0B6C
	float armor_regen_timeout; //0x0B70
	float inertia; //0x0B74
	float inertiaAccel; //0x0B78
	float jumpHeight; //0x0B7C
	float slopeSlowdown; //0x0B80
	float leanAngle; //0x0B84
	__int32 WeaponId; //0x0B88
	char pad_0x0B8C[0x100]; //0x0B8C
	float hasteTimeMult; //0x0C8C
	float hasteSpeedMult; //0x0C90
	float slideDistanceMult; //0x0C8C
	float minStaminaToStartHaste; //0x0C98
	char pad_0x0C9C[0x44]; //0x0C9C
	float knockBackResistance; //0x0CE0
};

class IActorIterator {
public:
	IActor* Next() { return vCall<IActor * (__thiscall*)(PVOID)>(this, 1)(this); }
};
class IActorSystem {
public:
	IActor* GetActor(EntityId entityId) { return vCall<IActor * (__thiscall*)(PVOID, EntityId)>(this, 15)(this, entityId); }
	void CreateActorIterator(IActorIterator** ActorIterator) { return vCall<void(__thiscall*)(PVOID, IActorIterator * *)>(this, 18)(this, ActorIterator); }
};

class IGameRules {
public:
	int CreateTeam(const char* name) { return vCall<int(__thiscall*)(PVOID, const char*)>(this, 98)(this, name); }
	int GetTeam(EntityId entityId) { return vCall<int(__thiscall*)(PVOID, EntityId)>(this, 108)(this, entityId); }
	void SetTeam(int teamId, EntityId entityId, bool clientOnly = false) { return vCall<void(__thiscall*)(PVOID, int, EntityId, bool)>(this, 106)(this, teamId, entityId, clientOnly); }


	char pad_0x0000[0x40]; //0x0000
	IGameFramework* m_pGameFramework; //0x0040 
	IGameplayRecorder* m_pGameplayRecorder; //0x0044 
	ISystem* m_pSystem; //0x0048 
	IActorSystem* m_pActorSystem; //0x004C 
	IEntitySystem* m_pEntitySystem; //0x0050 
	IScriptSystem* m_pScriptSystem; //0x0054 
	IMaterialManager* m_pMaterialManager; //0x0058 
	SmartScriptTable m_script; //0x005C 
	SmartScriptTable m_clientScript; //0x0060 
	SmartScriptTable m_serverScript; //0x0064 
	SmartScriptTable m_clientStateScript; //0x0068 
	SmartScriptTable m_serverStateScript; //0x006C 
};

class IGameFramework
{
public:
	IActorSystem* GetIActorSystem() { return pActorSystem; }
	bool GetClientActor(IActor** pActor) { return vCall<bool(__thiscall*)(PVOID, IActor * *)>(this, 142)(this, pActor); }
	IGameRules* GetIGameRulesSystem() { return *(IGameRules * *)((DWORD)this + 0x618); }
private:
	char _0x0000[1200];
	IActorSystem* pActorSystem; //0x04B0 
};
class IWeaponSystem
{
public:
	char pad_0x0000[0x4]; //0x0000
	IGame* m_pGame; //0x0004 
	ISystem* m_pSystem; //0x0008 
	IItemSystem* m_pItemSystem; //0x000C 
}; //Size=0x0010
class IGame
{
public:
	IWeaponSystem* GetIWeaponSystem() { return m_pWeaponSystem; }
	IGameFramework* GetIGameFramework() { return m_pGameFramework; }
	SCVars* GetGameVars() { return m_pSVars; }
private:
	char pad_0x0000[0x18]; //0x0000
	IWeaponSystem* m_pWeaponSystem; //0x0018 
	char pad_0x001C[0x8]; //0x001C
	IGameFramework* m_pGameFramework; //0x0024 
	char pad_0x0028[0x70]; //0x0028
	SCVars* m_pSVars; //0x0098
}; //Size=0x00A0


class SEntitySpawnParams
{
public:
	EntityId Id; //0x0000 
	char pad_0x0004[0x14]; //0x0004
	IEntityClass* pClass; //0x0018 
	char pad_0x001C[0x8]; //0x001C 
	const char* sName; //0x0024 
	char pad_0x0028[0x4]; //0x0028 
	DWORD nFlags; //0x002C 
	char pad_0x0030[0x10]; //0x0030 
	Vec3 vPosition; //0x0040 
	Quat qRotation; //0x004C 
	Vec3 vScale; //0x005C 
	char pad_0x00060[0x90]; //0x00060
}; //Size=0x0068
class IEntityClassRegistry
{
public:
	IEntityClass* FindClass(const char* sClassName) { return vCall<IEntityClass * (__thiscall*)(PVOID, const char*)>(this, 3)(this, sClassName); }
};
class IEntityIt
{
public:
	IEntity* Next() { return vCall<IEntity * (__thiscall*)(PVOID)>(this, 6)(this); }
};
class IEntitySystem
{
public:
	IEntityIt* GetEntityIterator() { return vCall<IEntityIt * (__thiscall*)(PVOID)>(this, 20)(this); }
	IEntity* GetEntity(EntityId Id) { return vCall<IEntity * (__thiscall*)(PVOID, EntityId)>(this, 11)(this, Id); }
	IEntityClassRegistry* GetClassRegistry() { return vCall<IEntityClassRegistry * (__thiscall*)(PVOID)>(this, 8)(this); }
	IEntity* SpawnEntity(SEntitySpawnParams& params, bool bAutoInit = true) { return vCall<IEntity * (__thiscall*)(PVOID, SEntitySpawnParams&, bool)>(this, 9)(this, params, bAutoInit); }
	IEntity* FindEntityByName(const char* sEntityName) { return vCall<IEntity * (__thiscall*)(PVOID, const char*)>(this, 42)(this, sEntityName); }
	IEntity* GetEntityFromPhysics(IPhysicalEntity* pPhysEntity) { return vCall<IEntity * (__thiscall*)(PVOID, IPhysicalEntity*)>(this, 25)(this, pPhysEntity); }
	void ObjectSpawn(const char* entityName, const char* entityClass, Vec3 vPos)
	{
		if (IEntityClass * pClass = GetClassRegistry()->FindClass(entityClass))
		{
			SEntitySpawnParams spawnParams;
			memset(&spawnParams, 0, sizeof(SEntitySpawnParams));
			spawnParams.vPosition = vPos;
			spawnParams.sName = entityName;
			spawnParams.pClass = pClass;
			SpawnEntity(spawnParams, true);
		}
	}
};

class IConsole
{
public:
	void ExecuteString(const char* command, const bool bSilentMode, const bool bDeferExecution = false)
	{
		return vCall<void(__thiscall*)(PVOID, const char*, const bool, const bool)>(this, 0x88 / 4)(this, command, bSilentMode, bDeferExecution);
	}
	ICVar* GetCVar(const char* Name)
	{
		return vCall<ICVar * (__thiscall*)(PVOID, const char*)>(this, 22)(this, Name);
	}
};

class IPhysicalWorld
{
public:
	int RayWorldIntersection(const Vec3& org, const Vec3& dir, int objtypes, unsigned int flags, ray_hit* hits, int nMaxHits, IPhysicalEntity* pSkipEnt, IPhysicalEntity* pSkipEntAux = 0, void* pForeignData = 0, int iForeignData = 0)
	{
		IPhysicalEntity* pSkipEnts[2];
		int nSkipEnts = 0;
		if (pSkipEnt) pSkipEnts[nSkipEnts++] = pSkipEnt;
		if (pSkipEntAux) pSkipEnts[nSkipEnts++] = pSkipEntAux;
		return RayWorldIntersection(org, dir, objtypes, flags, hits, nMaxHits, pSkipEnts, nSkipEnts, pForeignData, iForeignData);
	}
	int RayWorldIntersection(SRWIParams& rp, const char* pNameTag, int iCaller)
	{
		return vCall<int(__thiscall*)(PVOID, SRWIParams&, const char*, int)>(this, 68)(this, rp, pNameTag, iCaller);
	}
	int RayWorldIntersection(const Vec3& org, const Vec3& dir, int objtypes, unsigned int flags, ray_hit* hits, int nmaxhits, IPhysicalEntity** pSkipEnts = 0, int nSkipEnts = 0, void* pForeignData = 0, int iForeignData = 0, const char* pNameTag = "", ray_hit_cached * phitLast = 0, int iCaller = 4)
	{
		SRWIParams rp;
		rp.org = org;
		rp.dir = dir;
		rp.objtypes = objtypes;
		rp.flags = flags;
		rp.hits = hits;
		rp.nMaxHits = nmaxhits;
		rp.pForeignData = pForeignData;
		rp.iForeignData = iForeignData;
		rp.phitLast = phitLast;
		rp.pSkipEnts = pSkipEnts;
		rp.nSkipEnts = nSkipEnts;
		return RayWorldIntersection(rp, pNameTag, iCaller);
	}
};

class I3DEngine
{
public:
	void SetPostEffectParam(const char* pParam, float fValue, bool bForceValue = false)
	{
		return vCall<void(__thiscall*)(PVOID, const char*, float, bool)>(this, 143)(this, pParam, fValue, bForceValue);
	}
	void DeleteEntityDecals(IRenderNode* pEntity)
	{
		return vCall<void(__thiscall*)(PVOID, IRenderNode*)>(this, 158)(this, pEntity);
		//if (m_pDecalManager&& pEntity && (pEntity->m_nInternalFlags& IRenderNode::DECAL_OWNER)) m_pDecalManager->OnEntityDeleted(pEntity);
	}
};

class SSystemGlobalEnvironment {
public:
	IGame* GetIGame() { return *(IGame * *)((DWORD)this + 0x70); }
	char pad_0x0000[0x1C]; //0x0000
	ILog* pLog; //0x001C 
	IAISystem* pAISystem; //0x0020 
	char pad_0x0024[0x8]; //0x0024
	IScriptSystem* pScriptSystem; //0x002C 
	IRenderer* pRenderer; //0x0030 
	char pad_0x0034[0x10]; //0x0034
	IPhysicalWorld* pPhysicalWorld; //0x0044 
	IInput* pInput; //0x0048 
	char pad_0x004C[0x10]; //0x004C
	ICryPak* pCryPak; //0x005C 
	char pad_0x0060[0x8]; //0x0060
	ITimer* pTimer; //0x0068 
	IConsole* pConsole; //0x006C 
	IGame* pGame; //0x0070 
	char pad_0x0074[0x4]; //0x0074
	IEntitySystem* pEntitySystem; //0x0078 
	ISystem* pSystem; //0x0080 
	char pad_0x0084[0x4]; //0x0084
	I3DEngine* p3DEngine; //0x0088 
	char pad_0x008C[0x5C]; //0x008C
	char* CmdLine; //0x00E8 
	bool bServer; //0x0168 
	char pad_0x0169[0x17]; //0x0169
	__int32 os_ver; //0x0180 
	__int32 os_64; //0x0184 
};



class IItemParamsNode
{
public:
	bool GetAttribute(const char* name, int& attr)
	{
		return vCall<bool(__thiscall*)(PVOID, const char*, int&)>(this, 7)(this, name, attr);
		/*
		TAttributeMap::const_iterator it = FindAttrIterator(m_attributes, name);
		if (it != m_attributes.end()) return it->second.GetValueWithConversion(attr);
		return false;
		*/
	}
	const char* GetAttribute(const char* name)
	{
		return vCall<const char* (__thiscall*)(PVOID, const char*)>(this, 11)(this, name);
		/*TAttributeMap::const_iterator it = FindAttrIterator(m_attributes, name);
		if (it != m_attributes.end())
		{
			const string* str = it->second.GetPtr<string>();
			return str ? str->c_str() : 0;
		}
		return 0;*/
	}
	const char* GetNameAttribute()
	{
		return vCall<const char* (__thiscall*)(PVOID)>(this, 20)(this);
		//return m_nameAttribute.c_str();
	}
	int GetChildCount()
	{
		return vCall<int(__thiscall*)(PVOID)>(this, 21)(this);
		//return (int)m_children.size();
	}
	IItemParamsNode* GetChild(const char* name)
	{
		return vCall<IItemParamsNode * (__thiscall*)(PVOID, const char*)>(this, 23)(this, name);
		/*for (TChildVector::const_iterator it = m_children.begin(); it != m_children.end(); ++it)
			if (!strcmpi((*it)->GetName(), name))
				return (*it);
		return 0;*/
	}
	IItemParamsNode* GetChild(int i)
	{
		return vCall<IItemParamsNode * (__thiscall*)(PVOID, int)>(this, 24)(this, i);
		//if (i >= 0 && i < m_children.size()) return m_children[i];
		//return 0;
	}
	const char* GetName()
	{
		return vCall<const char* (__thiscall*)(PVOID)>(this, 30)(this);
		//return m_name.c_str(); 
	};
};
class IItemSystem
{
public:
	void SpawnAmmo(Vec3 vPos)
	{

	}
	//bullet_pt
	IItemParamsNode* GetItemParams(const char* itemName)
	{
		return vCall<IItemParamsNode * (__thiscall*)(PVOID, const char*)>(this, 8)(this, itemName);
	}
	void CacheObject(const char* fileName, bool useCgfStreaming)
	{
		return vCall<void(__thiscall*)(PVOID, const char*, bool)>(this, 28)(this, fileName, useCgfStreaming);
		/*
		string name = PathUtil::ToUnixPath(string(fileName));
		name.MakeLower();

		TObjectCacheIt oit = m_objectCache.find(name);
		TCharacterCacheIt cit = m_characterCache.find(name);

		if ((oit != m_objectCache.end()) || (cit != m_characterCache.end()))
		{
			return;
		}

		stack_string ext(PathUtil::GetExt(name.c_str()));

		if ((ext == "cdf") || (ext == "chr") || (ext == "cga"))
		{
			ICharacterInstance* pChar = gEnv->pCharacterManager->CreateInstance(fileName);
			if (pChar)
			{
				pChar->AddRef();
				m_characterCache.insert(TCharacterCache::value_type(name, pChar));
			}
		}
		else
		{
			IStatObj* pStatObj = gEnv->p3DEngine->LoadStatObj(fileName, 0, 0, useCgfStreaming);
			if (pStatObj)
			{
				pStatObj->AddRef();
				m_objectCache.insert(TObjectCache::value_type(name, pStatObj));
			}
		}
		*/
	}
	void CacheGeometry(IItemParamsNode* geometry)
	{
		//return vCall<void(__thiscall*)(PVOID, IItemParamsNode*)>(this, 29)(this, geometry);
		if (!geometry) return;
		int n = geometry->GetChildCount();
		if (n > 0)
		{
			for (int i = 0; i < n; i++)
			{
				IItemParamsNode* slot = geometry->GetChild(i);
				const char* name = slot->GetAttribute("name");
				int useStreaming = 1;
				slot->GetAttribute("useStreaming", useStreaming);
				if (name && name[0]) CacheObject(name, (useStreaming != 0));
			}
		}
	}
	void CacheItemGeometry(const char* className)
	{
		return vCall<void(__thiscall*)(PVOID, const char*)>(this, 30)(this, className);
		/*
		if (m_itemParamsFlushed) return;
		TItemParamsMap::iterator it = m_params.find(CONST_TEMP_STRING(className));
		if (it == m_params.end()) return;
		if ((it->second.precacheFlags & SItemParamsDesc::eIF_PreCached_Geometry) == 0)
		{
			if (const IItemParamsNode* root = GetItemParams(className))
			{
				const IItemParamsNode* geometry = root->GetChild("geometry");
				if (geometry) CacheGeometry(geometry);
			}
			it->second.precacheFlags |= SItemParamsDesc::eIF_PreCached_Geometry;
		}
		*/
	}
	void ClearGeometryCache()
	{
		return vCall<void(__thiscall*)(PVOID)>(this, 31)(this);
		/*
		for (TObjectCacheIt oit = m_objectCache.begin(); oit != m_objectCache.end(); ++oit)
		{
			if (oit->second)
				oit->second->Release();
		}
		m_objectCache.clear();

		for (TCharacterCacheIt cit = m_characterCache.begin(); cit != m_characterCache.end(); ++cit)
		{
			cit->second->Release();
		}
		m_characterCache.clear();

		for (TItemParamsMap::iterator it = m_params.begin(); it != m_params.end(); ++it)
		{
			it->second.precacheFlags &= ~SItemParamsDesc::eIF_PreCached_Geometry;
		}
		*/
	}
	void CacheItemSound(const char* className)
	{
		return vCall<void(__thiscall*)(PVOID, const char*)>(this, 32)(this, className);
		/*if (m_itemParamsFlushed) return;
		TItemParamsMap::iterator it = m_params.find(CONST_TEMP_STRING(className));
		if (it == m_params.end()) return;
		if ((it->second.precacheFlags & SItemParamsDesc::eIF_PreCached_Sound) == 0)
		{
			if (const IItemParamsNode * root = GetItemParams(className))
			{
				const IItemParamsNode* actions = root->GetChild("actions");
				if (actions)
				{
					int n = actions->GetChildCount();
					for (int i = 0; i < n; i++)
					{
						const IItemParamsNode* action = actions->GetChild(i);
						if (!stricmp(action->GetName(), "action"))
						{
							int na = action->GetChildCount();
							for (int a = 0; a < na; a++)
							{
								const IItemParamsNode* sound = actions->GetChild(i);
								if (!stricmp(sound->GetName(), "sound"))
								{
									const char* soundName = sound->GetNameAttribute();
									REINST("do we still need this type of data priming?")
									gEnv->pSoundSystem->Precache(soundName, 0, FLAG_SOUND_PRECACHE_EVENT_DEFAULT);
								}
							}
						}
					}

				}
			}

			it->second.precacheFlags |= SItemParamsDesc::eIF_PreCached_Sound;
		}*/
	}
	void ClearSoundCache()
	{
		return vCall<void(__thiscall*)(PVOID)>(this, 33)(this);
		//for (TItemParamsMap::iterator it = m_params.begin(); it != m_params.end(); ++it)
		//	it->second.precacheFlags &= ~SItemParamsDesc::eIF_PreCached_Sound;
	}

	IItem* GetItem(EntityId itemId)//22
	{
		auto v3 = *(DWORD*)((DWORD)this + 0x40);
		auto v5 = *(DWORD*)(v3 + 4);
		while (!*(BYTE*)(v5 + 13))
		{
			auto itemActId = *(DWORD*)(v5 + 16);
			if (itemActId == itemId)
			{
				return *(IItem * *)(v5 + 0x14);
				break;
			}
			else if (itemActId > itemId) v5 = *(DWORD*)v5;
			else v5 = *(DWORD*)(v5 + 8);
		}
		return nullptr;
	}
	EntityId GiveItem(IActor* pActor, const char* item, bool sound, bool select = true, bool keepHistory = true, const char* setup = NULL, EEntityFlags entityFlags = (EEntityFlags)0)//35
	{
		return vCall<EntityId(__thiscall*)(PVOID, IActor*, const char*, bool, bool, bool, const char*, EEntityFlags)>(this, 35)(this, pActor, item, sound, select, keepHistory, setup, entityFlags);

		/*if (!gEnv->bServer && !(entityFlags & ENTITY_FLAG_CLIENT_ONLY)) return 0;
		if (gEnv->pSystem->IsSerializingFile()) return 0;
		static char itemName[65];
		_sprintf(itemName, "%s%.03u", item, ++m_spawnCount);
		SEntitySpawnParams params;
		memset(&params, 0, sizeof(SEntitySpawnParams));
		params.sName = itemName;
		params.pClass = m_pEntitySystem->GetClassRegistry()->FindClass(item);
		params.nFlags |= 0x480000;// (ENTITY_FLAG_NO_PROXIMITY | ENTITY_FLAG_NEVER_NETWORK_STATIC | entityFlags);
		if (!params.pClass) return 0;
		if (IEntity* pItemEnt = m_pEntitySystem->SpawnEntity(params))
		{
			EntityId itemEntId = pItemEnt->GetId();
			IItem* pItem = GetItem(itemEntId);
			if (pItem)
			{
				pItem->PickUp(pActor->m_pEntity->GetId(), sound, select, keepHistory, setup);
				if (gEnv->pAISystem)
				{
					//if (IAIObject* pActorAI = pActor->GetEntity()->GetAI()) gEnv->pAISystem->SendSignal(SIGNALFILTER_SENDER, 0, "OnUpdateItems", pActorAI);
				}
				if ((pItemEnt = gEnv->pEntitySystem->GetEntity(itemEntId)) && !pItemEnt->IsGarbage())
				{
					if (pItemEnt->GetScriptTable()) pItemEnt->GetScriptTable()->SetToNull("Properties");
					return pItemEnt->GetId();
				}
			}
		}
		return 0;*/

	}
private:
	char pad_0x0004[0x1C]; //0x0004
	ISystem* m_pSystem; //0x0020 
	IGameFramework* m_pGameFramework; //0x0024 
	IEntitySystem* m_pEntitySystem; //0x0028 
	char pad_0x002C[0x28]; //0x002C
	uint32 m_spawnCount; //0x0054 
};

class SCVars
{
public:
	uint32 cl_payment_enabled; //0x0
	f32 cl_fov; //0x4
	char pad_0x008[4];
	f32 cl_tpvDist; //0xC
	f32 cl_tpvYaw; //0x10
	f32 cl_backpack_tpvYaw; //0x14
	f32 cl_nearPlane; //0x18
	f32 cl_sensitivity; //0x1C
	uint32 cl_invertMouse; //0x20
	uint32 cl_invertController; //0x24
	uint32 cl_crouchToggle; //0x28
	uint32 cl_zoomToggle; //0x2C
	uint32 cl_hasteToggle; //0x30
	uint32 cl_fpBody; //0x34
	uint32 cl_hud; //0x38
	uint32 cl_quick_play_only_pve; //0x3C
	uint32 cl_release_build; //0x40
	char pad_0x0044[16];
	f32 pl_curvingSlowdownSpeedScale; //0x54
	f32 ac_enableProceduralLeaning; //0x58
	f32 ac_msg_display_time; //0x5C
	char pad_0x0060[8];
	f32 cl_shallowWaterSpeedMulPlayer; //0x68
	f32 cl_shallowWaterSpeedMulAI; //0x6C
	f32 cl_shallowWaterDepthLo; //0x70
	f32 cl_shallowWaterDepthHi; //0x74
	f32 cl_selfSoundOffset; //0x78
	f32 cl_prone_hopping_spread; //0x7C
	uint32 cl_prone_hopping_improvement; //0x80
	uint32 cl_invisible_character_anim_fix; //0x84
	uint32 cl_banners_enabled; //0x88
	uint32 cl_banners_enabled_update; //0x8C
	uint32 cl_banners_enabled_debug; //0x90
	uint32 cl_banners_enabled_external_path; //0x94
	f32 cl_banners_update_timeout; //0x98
	uint32 g_rating_game_restricted_equipment_verification_enabled; //0x9C
	uint32 cl_shop_skin_validation_enabled; //0xA0
	uint32 cl_shop_hidden_category; //0xA4
	uint32 cl_enable_personalized_offers; //0xA8
	uint32 sv_voting_timeout; //0xAC
	uint32 sv_voting_cooldown; //0xB0
	f32 sv_voting_ratio; //0xB4
	f32 sv_voting_team_ratio; //0xB8
	uint32 sv_input_timeout; //0xBC
	uint32 sv_cvars_hash_enable; //0xC0
	uint32 sv_empty_grenade_fix; //0xC4
	f32 hr_rotateFactor; //0xC8
	f32 hr_rotateTime; //0xCC
	uint32 i_soundeffects; //0xD0
	uint32 i_lighteffects; //0xD4
	uint32 i_particleeffects; //0xD8
	uint32 i_rejecteffects; //0xDC
	f32 i_offset_front; //0xE0
	f32 i_offset_up; //0xE4
	f32 i_offset_right; //0xE8
	char pad_0x00EC[12];
	f32 i_rotation_yaw; //0xF8
	f32 i_rotation_pitch; //0xFC
	f32 i_rotation_roll; //0x100
	char pad_0x00104[12];
	f32 i_zoomed_inertia; //0x110
	f32 i_zoomed_inertia_blend; //0x114
	f32 i_pelletsDisp; //0x118
	f32 i_offset_nearFOV; //0x11C
	uint32 i_unlimitedammo; //0x120
	f32 pl_inputAccel; //0x124
	uint32 g_detachCamera; //0x128
	char pad_0x0012C[16];
	uint32 g_playerHealthValue; //0x13C
	uint32 g_flashlightEnable; //0x140
	f32 g_maxPlayerSpeed; //0x144
	uint32 g_highlightPickupItems; //0x148
	f32 g_stanceTransitionSpeed; //0x14C
	f32 g_proneTransitionSpeed; //0x150
	char pad_0x00154[4];
	uint32 g_enableIdleCheck; //0x158
	f32 g_playerLowHealthThreshold; //0x15C
	f32 g_aoe_melee_view_cone_angle; //0x160
	f32 g_melee_view_cone_angle; //0x164
	uint32 sv_pacifist; //0x168
	uint32 g_ec_enable; //0x16C
	f32 g_ec_radiusScale; //0x170
	f32 g_ec_volume; //0x174
	f32 g_ec_extent; //0x178
	uint32 g_ec_removeThreshold; //0x17C
	f32 g_radialBlur; //0x180
	f32 g_timelimit; //0x184
	f32 g_roundtime; //0x188
	uint32 g_preroundtime; //0x18C
	uint32 g_postgametime; //0x190
	uint32 g_roundlimit; //0x194
	uint32 g_scorelimit; //0x198
	uint32 g_fraglimit; //0x19C
	uint32 g_fraglead; //0x1A0
	f32 g_friendlyfireratio; //0x1A4
	uint32 g_revivetime; //0x1A8
	uint32 g_minteamlimit; //0x1AC
	char pad_0x001B0[4];
	uint32 g_awardTeamScoreAtRoundEnd; //0x1B4
	uint32 g_locked_spectator_camera; //0x1B8
	uint32 g_overtime_rounds_limit; //0x1BC
	f32 g_min_core_count_modifier; //0x1C0
	f32 g_max_core_count_modifier; //0x1C4
	f32 g_core_score_producing_speed; //0x1C8
	uint32 g_core_reset_time; //0x1CC
	uint32 g_core_capture_time; //0x1D0
	uint32 g_speed_up_capturing_for_losers; //0x1D4
	uint32 g_debug_cores_capturing; //0x1D8
	uint32 g_spawn_groups_shuffle_timeout; //0x1DC
	uint32 g_debug_spawn_groups_shuffle; //0x1E0
	uint32 g_dmn_distance_weighted_spawn; //0x1E4
	uint32 g_session_ending_round_limit; //0x1E8
	uint32 g_session_ending_round_limit_change; //0x1EC
	f32 g_session_ending_fraglimit; //0x1F0
	f32 g_session_ending_timelimit; //0x1F4
	f32 g_session_ending_dst_cores_limit; //0x1F8
	uint32 sv_session_progress_debug; //0x1FC
	uint32 sv_serverinfo_tick_timeout; //0x200
	uint32 sv_serverinfo_track_timeout; //0x204
	uint32 sv_serverinfo_switch_timeout; //0x208
	uint32 sv_serverinfo_close_server; //0x20C
	char pad_0x00210[8];
	f32 g_protection_inertia; //0x218
	uint32 g_enable_flashbang_mechanic; //0x21C
	char pad_0x00220[8];
	uint32 g_skipIntro; //0x228
	uint32 g_resetActionmapOnStart; //0x22C
	uint32 g_useProfile; //0x230
	uint32 g_startFirstTime; //0x234
	uint32 g_enableTracers; //0x238
	uint32 g_parametric_recoil_random_enabled; //0x23C
	uint32 g_parametric_spread_attack_enabled; //0x240
	uint32 g_contracts_set_size; //0x244
	char pad_0x00248[4];
	uint32 cl_useLoginScreen; //0x24C
	f32 cl_repair_introduction_threshold; //0x250
	uint32 g_pve_mission_access_limitation_enabled; //0x254
	char pad_0x00258[4];
	uint32 pl_perframe_update_players; //0x25C
	uint32 pl_perframe_update_ai; //0x260
	char pad_0x00264[4];
	uint32 g_autoplay_enable; //0x268
	uint32 g_autoplay_ignoreLinkName; //0x26C
	uint32 g_autoplay_separate_client_stats; //0x270
	uint32 g_autoplay_enable_knockback; //0x274
	uint32 g_autoplay_active; //0x278
	uint32 g_autoplay_shoot; //0x27C
	uint32 g_autoplay_shoot_if_visible; //0x280
	uint32 g_autoplay_knockback_killzone; //0x284
	uint32 g_autoplay_set_path; //0x288
	uint32 g_autoplay_randomization; //0x28C
	uint32 g_autoplay_exectime; //0x290
	uint32 g_autoplay_use_zoom; //0x294
	uint32 g_autoplay_pause; //0x298
	f32 g_autoplay_minspeed; //0x29C
	f32 g_autoplay_stuck_move_time; //0x2A0
	f32 g_autoplay_stuck_teleport_time; //0x2A4
	uint32 g_autoplay_randomization_ignore_first; //0x2A8
	uint32 g_autoplay_randomization_ignore_last; //0x2AC
	uint32 g_autoplay_randomization_angle; //0x2B0
	uint32 g_autoplay_randomization_movement; //0x2B4
	uint32 g_autoplay_quit; //0x2B8
	uint32 g_autoplay_respawn; //0x2BC
	uint32 g_autoplay_aim_players; //0x2C0
	uint32 g_autoplay_turn_speed; //0x2C4
	uint32 g_autoplay_set_closest_path; //0x2C8
	uint32 g_autoplay_log; //0x2CC
	uint32 g_autoplay_repeat; //0x2D0
	uint32 g_autoplay_reload_on_respawn; //0x2D4
	char pad_0x002D8[12];
	uint32 g_weapon_system_preload; //0x2E4
	uint32 g_weapon_system_verbose; //0x2E8
	f32 pl_fallDamage_SpeedSafe; //0x2EC
	f32 pl_fallDamage_SpeedFatal; //0x2F0
	char pad_0x002F4[8];
	f32 pl_fallDamage_SpeedBias; //0x2FC
	f32 pl_velocityInterpAirControlScale; //0x300
	char pad_0x00304[4];
	f32 pl_velocityInterpDesiredPosThreshold; //0x308
	f32 pl_netSerialiseMaxSpeed; //0x30C
	uint32 pl_defer_actions_on_death; //0x310
	uint32 pl_extra_fp_spectator_latency; //0x314
	uint32 pl_serialisePhysVel; //0x318
	f32 pl_remoteInertia; //0x31C
	uint32 pl_newInertia; //0x320
	f32 pl_impulse_recover; //0x324
	uint32 pl_inertia_haste_enabled; //0x328
	uint32 pl_lerp_method; //0x32C
	uint32 pl_lerp_rmi; //0x330
	f32 pl_lerp_depth; //0x334
	f32 pl_lerp_ahead; //0x338
	f32 pl_lerp_max_speed_threshold_mult; //0x33C
	f32 pl_lerp_snap_server_jump_distance; //0x340
	f32 pl_lerp_snap_remote_jump_distance; //0x344
	f32 pl_lerp_damp_extrapolation; //0x348
	uint32 pl_jump_fix; //0x34C
	uint32 pl_jump_validate_position; //0x350
	f32 pl_netAimLerpFactor; //0x354
	uint32 hud_hit_indicator_threshold_angle; //0x358
	f32 hud_hit_indicator_mult_r; //0x35C
	f32 hud_hit_indicator_mult_w; //0x360
	f32 hud_hit_indicator_mult_h; //0x364
	uint32 hud_mpNamesDuration; //0x368
	uint32 hud_mpNamesNearDistance; //0x36C
	uint32 hud_mpNamesFarDistance; //0x370
	uint32 hud_onScreenNearDistance; //0x374
	uint32 hud_onScreenFarDistance; //0x378
	f32 hud_onScreenNearSize; //0x37C
	f32 hud_onScreenFarSize; //0x380
	uint32 hud_godFadeTime; //0x384
	uint32 hud_crosshair; //0x388
	uint32 hud_chDamageIndicator; //0x38C
	uint32 hud_panoramicHeight; //0x390
	f32 hud_teammateBadgeMultiplier; //0x394
	f32 hud_teammateBadgeRadius; //0x398
	uint32 hud_subtitlesRenderMode; //0x39C
	uint32 hud_subtitlesHeight; //0x3A0
	uint32 hud_subtitlesFontSize; //0x3A4
	uint32 hud_subtitlesQueueCount; //0x3A8
	uint32 hud_subtitlesVisibleCount; //0x3AC
	uint32 hud_aspectCorrection; //0x3B0
	f32 hud_ctrl_Curve_X; //0x3B4
	f32 hud_ctrl_Curve_Z; //0x3B8
	f32 hud_ctrl_Coeff_X; //0x3BC
	f32 hud_ctrl_Coeff_Z; //0x3C0
	uint32 hud_ctrlZoomMode; //0x3C4
	uint32 hud_attachBoughEquipment; //0x3C8
	uint32 hud_startPaused; //0x3CC
	uint32 hud_show_flag_status; //0x3D0
	uint32 hud_show_has_flag; //0x3D4
	uint32 hud_show_bomb_status; //0x3D8
	uint32 hud_show_has_bomb; //0x3DC
	uint32 hud_show_cores_status; //0x3E0
	uint32 hud_show_dst_status; //0x3E4
	uint32 hud_show_dmn_status; //0x3E8
	uint32 hud_show_team_scores; //0x3EC
	uint32 hud_show_team_kills; //0x3F0
	uint32 hud_no_team_pvp; //0x3F4
	uint32 hud_show_total_kills; //0x3F8
	uint32 hud_show_mission_progress; //0x3FC
	uint32 hud_round_timer_direction_forward; //0x400
	uint32 hud_round_timer_countdown_start_time; //0x404
	uint32 hud_display_enemy_outlines; //0x408
	uint32 hud_show_zsd_status; //0x40C
	uint32 hud_show_objective_state_bar; //0x410
	uint32 hud_show_team_health_status; //0x414
	uint32 hud_show_ingame_currency_status; //0x418
	uint32 hud_show_alive_players_counter; //0x41C
	uint32 hud_show_consumables; //0x420
	uint32 hud_secondary_objectives_delay; //0x424
	uint32 hud_for_observer_enabled; //0x428
	uint32 hud_combat_log_flash; //0x42C
	f32 tracer_min_distance; //0x430
	f32 tracer_max_distance; //0x434
	f32 tracer_min_scale; //0x438
	f32 tracer_max_scale; //0x43C
	uint32 tracer_max_count; //0x440
	f32 tracer_player_radiusSqr; //0x444
	uint32 i_debug_projectiles; //0x448
	f32 i_weapon_raise_distance; //0x44C
	uint32 i_enable_inventory_select; //0x450
	uint32 g_enable_ingame_kickvote_in_zombiemission; //0x454
	uint32 g_kickvote_pve_max_checkpoints; //0x458
	uint32 g_proneNotUsableWeapon_FixType; //0x45C
	uint32 g_proneAimAngleRestrict_Enable; //0x460
	uint32 g_enableFriendlyFallAndPlay; //0x464
	uint32 g_spectate_TeamOnly; //0x468
	uint32 g_placeable_explosives_limit_per_type; //0x46C
	uint32 g_MPDeathCam; //0x470
	f32 g_death_cam_time; //0x474
	f32 g_death_cam_focus_time; //0x478
	f32 g_death_cam_interpolate_pos_speed; //0x47C
	f32 g_death_cam_focus_hold_time; //0x480
	f32 g_death_cam_enemy_height_to_view_height_proportion; //0x484
	f32 g_death_cam_focus_enemy_view_offset; //0x488
	f32 g_death_cam_interpolate_direction_speed; //0x48C
	f32 g_death_cam_input_blending_time; //0x490
	uint32 g_inputSerialization; //0x494
	uint32 g_physicsSerialization; //0x498
	uint32 g_physicsSerializationMode; //0x49C
	char pad_0x004A0[16];
	uint32 g_teams; //0x4B0
	uint32 g_spawnMode; //0x4B4
	uint32 g_spawn_count_max_random; //0x4B8
	uint32 g_allow_multiple_revives; //0x4BC
	char pad_0x004C0[8];
	uint32 g_result_view_max_spawns; //0x4C8
	char pad_0x004CC[16];
	uint32 g_coopmoves_ignore_static_geom; //0x4DC
	uint32 g_dedicated_disable_physics; //0x4E0
	char pad_0x004E4[4];
	uint32 g_pushable_restore; //0x4E8
	uint32 g_pushable_forcemode; //0x4EC
	uint32 g_pushable_physicsmode; //0x4F0
	uint32 g_pushable_startmode; //0x4F4
	f32 g_pushable_startdelay; //0x4F8
	f32 g_pushingPlayerOffset; //0x4FC
	f32 g_pushable_force_growing_time; //0x500
	char pad_0x00504[40];
	uint32 g_smUseGeometryCollisionTest; //0x52C
	uint32 g_smGeometryCollisionWarnings; //0x530
	uint32 g_smGeometryCollisionCoopAssistCapsuleCheck; //0x534
	f32 g_smAssistStartDistIncr; //0x538
	uint32 g_smUseLedgeObjects; //0x53C
	uint32 g_smUseLedgeValidation; //0x540
	f32 g_smValidationDrawThreshold; //0x544
	char pad_0x00548[4];
	uint32 g_crosshairInScopeZoom_Enable; //0x54C
	uint32 g_crosshairSpreadTuning_Enable; //0x550
	uint32 g_useAnglesSmoothClamp; //0x554
	f32 g_anglesSmoothClampExp; //0x558
	f32 g_flagSpeedScale; //0x55C
	f32 g_flagDamageReduction; //0x560
	uint32 g_flagResetTime; //0x564
	uint32 g_flagVisibilityDist; //0x568
	uint32 g_swapAttackers; //0x56C
	uint32 g_comaTime; //0x570
	uint32 g_coma_time_class_change; //0x574
	f32 g_aimedShotRecoil; //0x578
	uint32 g_victoryCondition; //0x57C
	uint32 g_useNewStormLogic; //0x580
	uint32 g_ranked; //0x584
	f32 g_explosiveLifeTimeAfterDeath; //0x588
	char pad_0x0058C[4];
	uint32 g_allow_attachments_pickup; //0x590
	uint32 g_hide_locked_attachments; //0x594
	uint32 gameK01; //0x598
	uint32 g_cheats; //0x59C
	uint32 g_godMode; //0x5A0
	char pad_0x005A4[16];
	uint32 g_demo_mode; //0x5B4
	uint32 pl_fp_gender; //0x5B8
	f32 pl_fp_fatness; //0x5BC
	char pad_0x005C0[4];
	uint32 g_cache_itemcc; //0x5C4
	uint32 g_cache_actorcc; //0x5C8
	uint32 g_cache_warn_memlimit; //0x5CC
	char pad_0x005D0[12];
	uint32 ui_separate_thousands; //0x5DC
	uint32 ui_announcement_message_length_limit; //0x5E0
	uint32 ui_mipmaps; //0x5E4
	uint32 ui_screen_instant_delete; //0x5E8
	uint32 ui_tooltips; //0x5EC
	uint32 ui_hover_delay; //0x5F0
	char pad_0x005F4[4];
	uint32 ui_lobby_getrooms_request_size; //0x5F8
	f32 ui_lobby_autorefresh; //0x5FC
	f32 ui_damageinfo_delay; //0x600
	char pad_0x00604[8];
	uint32 ui_bag_default_size; //0x60C
	uint32 ui_randombox_machine_buy_delay; //0x610
	f32 ui_transparency_zone_size; //0x614
	f32 ui_transparency_zone_min_alpha; //0x618
	f32 ui_transparency_zone_max_alpha; //0x61C
	char pad_0x00620[40];
	f32 ui_area_name_fade_delay; //0x648
	uint32 ui_area_name_enabled; //0x64C
	uint32 ui_radar; //0x650
	f32 ui_radar_scale; //0x654
	f32 ui_kickvote_close_timeout; //0x658
	char pad_0x0065C[16];
	f32 ui_remote_player_info_request_delay; //0x66C
	f32 ui_switch_pve_map_delay; //0x670
	f32 ui_random_box_fall_delay; //0x674
	f32 ui_achievement_notification_delay; //0x678
	uint32 ui_ingame_scoreboard_enabled; //0x67C
	f32 ui_ingame_scoreboard_duration; //0x680
	uint32 ui_chat_merge_channels; //0x684
	char pad_0x00688[4];
	uint32 ui_reward_max_visible_players; //0x68C
	uint32 ui_reward_data_timeout; //0x690
	uint32 ui_reward_close_timeout; //0x694
	uint32 ui_crown_reward_close_timeout; //0x698
	uint32 ui_reward_animations_length; //0x69C
	uint32 ui_tutorial_mandatory; //0x6A0
	uint32 ui_reward_support_multiplier; //0x6A4
	uint32 ui_lobby_survival_tab_enabled; //0x6A8
	uint32 ui_lobby_battle_pass_tab_enabled; //0x6AC
	uint32 ui_lobby_services_tab_enabled; //0x6B0
	uint32 ui_lobby_rating_tab_min_rank; //0x6B4
	uint32 ui_surrender_vote_enabled; //0x6B8
	uint32 ui_preinvite_enabled; //0x6BC
	uint32 ui_preinvite_force_start; //0x6C0
	uint32 ui_auto_repair_notification_delay; //0x6C4
	uint32 ui_preinvite_start_timeout; //0x6C8
	uint32 g_preinvite_ping_timeout; //0x6CC
	uint32 ui_money_refill_enable; //0x6D0
	uint32 ui_money_refill_min_rank; //0x6D4
	uint32 ui_money_refill_min_repair_percent; //0x6D8
	uint32 ui_show_average_searching_time; //0x6DC
	char pad_0x006E0[4];
	uint32 ui_randombox_choice_limitation_enabled; //0x6E4
	uint32 ui_voip_enabled; //0x6E8
	uint32 ui_mailbox_enabled; //0x6EC
	uint32 g_gameroom_countdown; //0x6F0
	uint32 g_gameroom_afk_timeout; //0x6F4
	uint32 g_ingame_afk_timeout; //0x6F8
	uint32 g_afk_debug; //0x6FC
	char pad_0x00700[28];
	f32 g_hitValidationDefaultLifeTime; //0x71C
	f32 g_hitValidationTimeout; //0x720
	f32 g_lobbyFOV; //0x724
	f32 g_resultFOV; //0x728
	f32 i_weaponNetDelay; //0x72C
	char pad_0x00730[8];
	uint32 g_drawNearClippingPlane; //0x738
	uint32 i_melee_aoe_attack_algorithm; //0x73C
	uint32 i_pickupObjectsMaxCount; //0x740
	f32 i_pickupObjectsMaxTime; //0x744
	f32 i_pickupDistance; //0x748
	f32 i_pickupCylinderRadius; //0x74C
	char pad_0x00750[4];
	f32 i_quickCustomizeDelay; //0x754
	char pad_0x00758[12];
	f32 e_statsflush_period; //0x764
	uint32 g_metrics_enabled; //0x768
	uint32 g_metrics_debug; //0x76C
	uint32 g_metrics_timeout_ms; //0x770
	char pad_0x00774[4];
	uint32 g_metrics_sink_port; //0x778
	uint32 g_hardwarestats; //0x77C
	f32 g_hardwarestats_update_persistent_interval; //0x780
	f32 g_hardwarestats_update_varying_interval; //0x784
	f32 g_hardwarestats_send_varying_interval; //0x788
	f32 g_hardwarestats_send_telem_interval; //0x78C
	f32 g_hardwarestats_gpulimit_threshold; //0x790
	char pad_0x00794[4];
	uint32 g_hardwarestats_pos_cell_size; //0x798
	uint32 g_hardwarestats_cpu_quantize_level; //0x79C
	uint32 g_hardwarestats_mem_quantize_level; //0x7A0
	uint32 g_hardwarestats_screen_on_peaks; //0x7A4
	f32 g_hardwarestats_screen_timeout; //0x7A8
	uint32 g_hardwarestats_session_id; //0x7AC
	f32 g_comfortFPS; //0x7B0
	f32 g_comfortFPSInterval; //0x7B4
	char pad_0x007B8[4];
	f32 g_armorAbsorb; //0x7BC
	char pad_0x007C0[4];
	uint32 cl_feedback_system_debug; //0x7C4
	char pad_0x007C8[4];
	uint32 g_tutorial_confirmation; //0x7CC
	f32 g_demo_playback_rotation_smooth; //0x7D0
	uint32 sv_disconnect_to_lobby; //0x7D4
	f32 sv_c4timer; //0x7D8
	uint32 pl_accelMultComput; //0x7DC
	uint32 g_pve_end_when_all_dead; //0x7E0
	uint32 g_pve_num_players; //0x7E4
	char pad_0x007E8[4];
	uint32 ach_streak_interval; //0x7EC
	f32 g_instant_award_delay; //0x7F0
	uint32 g_background_server_port; //0x7F4
	uint32 g_background_server_hint; //0x7F8
	uint32 g_background_server_hmid; //0x7FC
	char pad_0x00800[8];
	f32 ai_PlayerStationaryTime; //0x808
	f32 pl_stationary_threshold; //0x80C
	uint32 ai_global_limit; //0x810
	uint32 g_VisMaskOR; //0x814
	uint32 ai_draw_path; //0x818
	uint32 g_narrator_enable; //0x81C
	uint32 g_crowd_enable; //0x820
	uint32 g_dropWeaponOnDeath; //0x824
	f32 g_pve_teleport_refill_ratio; //0x828
	f32 g_pve_teleport_ready_ratio; //0x82C
	f32 g_pve_teleport_ready_timeout; //0x830
	f32 g_pve_teleport_fadein_duration; //0x834
	uint32 g_reconnect_to_game_enabled; //0x838
	char pad_0x0083C[48];
	uint32 online_use_query_lb; //0x86C
	char pad_0x00870[4];
	uint32 online_use_protect; //0x874
	uint32 online_check_certificate; //0x878
	uint32 online_send_delay; //0x87C
	uint32 online_masterserver_rotate; //0x880
	uint32 online_newbie_rank; //0x884
	uint32 online_pvp_rank; //0x888
	uint32 online_thread_mode; //0x88C
	char pad_0x00890[68];
	uint32 g_mission_release_mission; //0x8D4
	uint32 g_mission_clanwar_mission; //0x8D8
	uint32 g_mission_only_clanwar_mission; //0x8DC
	uint32 g_mission_apply_from_level; //0x8E0
	uint32 g_enableChangeClass; //0x8E4
	uint32 g_classSwitchOnRoundEnd; //0x8E8
	uint32 g_classSwitchOnRespawn; //0x8EC
	f32 g_classSwitchTime; //0x8F0
	uint32 g_characterCacheSize; //0x8F4
	char pad_0x008F8[8];
	f32 ai_spawn_timeout; //0x900
	char pad_0x00904[8];
	uint32 g_client_cinematics; //0x90C
	uint32 g_client_cinematics_serialization_max; //0x910
	char pad_0x00914[4];
	f32 g_client_cinematics_correction_threshold; //0x918
	f32 g_cinematicTriggerAccelMult2; //0x91C
	f32 g_cinematicTriggerAccelMult3; //0x920
	f32 g_cinematicTriggerAccelMult4; //0x924
	char pad_0x00928[8];
	f32 g_invulnerabilityTime; //0x930
	f32 g_coinInvulnerabilityTime; //0x934
	uint32 g_ppes_enabled; //0x938
	f32 g_ppes_decay; //0x93C
	uint32 g_ppes_decay_cooldown; //0x940
	uint32 g_ppes_mult_enabled; //0x944
	uint32 g_ppes_mult_step; //0x948
	char pad_0x0094C[8];
	uint32 g_wait_all_loaded_timeout; //0x954
	uint32 g_pvp_end_when_enemy_team_empty; //0x958
	uint32 g_postroundtime; //0x95C
	uint32 g_dst_score_limit; //0x960
	uint32 g_dst_score_increment; //0x964
	uint32 g_dst_score_kill; //0x968
	uint32 g_dst_capture_time; //0x96C
	uint32 g_dst_segments_count; //0x970
	uint32 g_dst_timelimit; //0x974
	f32 g_bomb_pickup_timeout; //0x978
	f32 ui_hitfeedbackmax; //0x97C
	uint32 g_hit_feedback_event; //0x980
	f32 g_hostMigrationResumeTime; //0x984
	uint32 g_hostMigrationDebug; //0x988
	char pad_0x0098C[20];
	uint32 g_chat_level_threshold; //0x9A0
	uint32 g_chat_timeout_public_low_level; //0x9A4
	uint32 g_chat_timeout_public_high_level; //0x9A8
	uint32 g_chat_timeout_ingame; //0x9AC
	uint32 g_chat_enabled_ingame; //0x9B0
	uint32 g_chat_language_separated; //0x9B4
	uint32 cl_first_start_sys_spec; //0x9B8
	f32 g_coma_pre_revive_time; //0x9BC
	f32 g_coma_pre_revive_time_penalty; //0x9C0
	f32 g_coma_post_resurrect_time; //0x9C4
	uint32 g_ammo_pack_restore_threshold; //0x9C8
	uint32 g_low_on_ammo_threshold; //0x9CC
	f32 g_spectator_dead_player_timeout; //0x9D0
	f32 g_spectator_camera_hor_sens; //0x9D4
	f32 g_spectator_camera_vert_sens; //0x9D8
	f32 g_spectator_camera_vert_ang_top; //0x9DC
	f32 g_spectator_camera_vert_ang_bottom; //0x9E0
	f32 g_spectator_camera_vert_offset; //0x9E4
	f32 g_spectator_camera_dist; //0x9E8
	f32 g_spectator_camera_interpolation_speed; //0x9EC
	f32 g_spectator_camera_thirdperson_interpolation_speed; //0x9F0
	f32 g_spectator_camera_rotate_x_angle; //0x9F4
	f32 g_spectator_camera_radius_sweep_test; //0x9F8
	f32 g_spectator_camera_z_offset_interpolate_speed; //0x9FC
	uint32 g_spectator_fp_enable; //0xA00
	uint32 g_spectator_death_cam_enable; //0xA04
	f32 g_observer_freecam_fly_speed; //0xA08
	f32 g_observer_freecam_haste_speed_mult; //0xA0C
	f32 g_observer_freecam_vertical_speed; //0xA10
	f32 g_clientDfConeAngle; //0xA14
	f32 g_serverDfConeAngle; //0xA18
	f32 g_interaction_convergence_speed; //0xA1C
	f32 g_interaction_cinematic_delay; //0xA20
	uint32 cl_show_hit_feedback_time; //0xA24
	uint32 g_anti_cheat_memscan_thread_delay; //0xA28
	uint32 g_anti_cheat_memscan_log; //0xA2C
	uint32 g_anti_cheat_memscan_dump_patterns; //0xA30
	char pad_0x00A34[20];
	uint32 g_ray_bullet; //0xA48
	uint32 g_ray_bullet_fast; //0xA4C
	f32 g_ray_bullet_range; //0xA50
	uint32 g_multi_shoot_rmi; //0xA54
	uint32 g_shoot_hit_kill_rmi_unordered_fast; //0xA58
	char pad_0x00A5C[8];
	f32 g_movement_silent_resync_thr; //0xA64
	f32 g_movement_delay_resync_thr; //0xA68
	f32 g_movement_delay_resync_time; //0xA6C
	f32 g_movement_validation_thr; //0xA70
	uint32 g_movement_estimation_enabled; //0xA74
	f32 g_movement_estimation_period; //0xA78
	f32 g_player_badge_resync_distance; //0xA7C
	f32 g_precache_weapons_jobs_max_delta_time; //0xA80
	char pad_0x00A84[12];
	uint32 g_auto_zoom_after_reload; //0xA90
	f32 g_zoom_speed_multiplier; //0xA94
	char pad_0x00A98[4];
	uint32 ui_localserver; //0xA9C
	char pad_0x00AA0[4];
	uint32 g_coins_usage_limit_enabled; //0xAA4
	uint32 g_coins_usage_limit; //0xAA8
	uint32 g_coins_refill_ammo; //0xAAC
	uint32 g_coins_resurrect_at_spawn; //0xAB0
	uint32 ai_smoke_obstruct; //0xAB4
	uint32 g_perf_stat_collection_enable; //0xAB8
	uint32 g_perf_stat_max_packets_per_frame; //0xABC
	f32 g_perf_stat_packet_send_offset; //0xAC0
	char pad_0x00AC4[84];
	uint32 g_rigid_body_cinematics; //0xB18
	uint32 g_cinematics_body_sleeping_enable; //0xB1C
	uint32 g_cinematics_body_sleeping_warmup_time; //0xB20
	uint32 g_soft_player_collision; //0xB24
	char pad_0x00B28[4];
	f32 i_smoothed_fire_buffer_interval; //0xB2C
	f32 cl_stand_to_crouch_delay; //0xB30
	f32 cl_stand_to_prone_delay; //0xB34
	f32 cl_crouch_to_stand_delay; //0xB38
	f32 cl_crouch_to_prone_delay; //0xB3C
	f32 cl_prone_to_stand_delay; //0xB40
	f32 cl_prone_to_crouch_delay; //0xB44
	char pad_0x00B48[4];
	uint32 g_patch_head_pos; //0xB4C
	f32 g_patch_head_pos_max_offset; //0xB50
	f32 g_patch_head_pos_max_pitch; //0xB54
	uint32 cl_coop_mode_switch_delay; //0xB58
	f32 g_resurrect_rwi_offset; //0xB5C
	f32 g_maxTimeBetweenAttackAndKillForAssist; //0xB60
	f32 g_ingame_currency_multiplier; //0xB64
	uint32 g_ingame_currency_boost_type; //0xB68
	char pad_0x00B6C[4];
	uint32 g_ragdoll_collision_group; //0xB70
	f32 g_hide_spawn_anim_tpose_offset; //0xB74
	uint32 g_hide_spawn_anim_tpose_debug; //0xB78
	f32 g_SM_PlLowHealth_TimeBetweenActivations; //0xB7C
	uint32 g_SM_PlLowHealth_DamageOfTotalHPPercent; //0xB80
	uint32 g_SM_PlLowHealth_LowHealth; //0xB84
	uint32 g_dogtags; //0xB88
	uint32 g_dogtags_limit; //0xB8C
	uint32 g_dogtags_lifetime; //0xB90
	f32 g_dogtags_pickup_time; //0xB94
	uint32 g_silencer_shoot_spotting; //0xB98
	uint32 g_bomb_drop_enabled; //0xB9C
	f32 g_spawn_anticlaymore_buff_delay; //0xBA0
	f32 g_spawn_anticlaymore_buff_mul; //0xBA4
	char pad_0x00BA8[16];
	uint32 g_enable_function_time_collector; //0xBB8
	uint32 g_merged_hit_kill; //0xBBC
	char pad_0x00BC0[4];
	uint32 g_sm_haste_serialize; //0xBC4
	uint32 g_use_ai_spawn_presets_on_export_debug; //0xBC8
	uint32 g_teambalance_enabled; //0xBCC
	uint32 g_projectile_interpolation; //0xBD0
	uint32 sv_invalid_position_fix_mode; //0xBD4
	f32 sv_invalid_position_fix_z_threshold; //0xBD8
	char pad_0x00BDC[8];
	uint32 g_player_smoke_obstruct; //0xBE4
	uint32 g_defending_team_show_bomb_marker; //0xBE8
	f32 g_enemy_coma_marker_duration; //0xBEC
	uint32 g_random_item_spawn_max; //0xBF0
	uint32 i_log_custom_animation; //0xBF4
	uint32 g_debug_killzone_grenade; //0xBF8
	uint32 ai_throw_grenade_delay; //0xBFC
};
