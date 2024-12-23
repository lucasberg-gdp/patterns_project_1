#include "cLuaBrain.h"

#include <iostream>
#include "../cMesh.h"
#include "../cGameManager.h"
#include "../cCommand_MoveTo.h"

#include "Lua5.4.6/lua.h"

//#include "../cGameObject.h"

// Function in the main
//int lua_ChangeTaylorSwiftTexture(lua_State* L);
//int lua_AddSerialMoveObjectCommand(lua_State* L);
//
//int lua_SetMeshPositionByFriendlyName(lua_State* L);
//int lua_GetMeshPositionByFriendlyName(lua_State* L);

extern std::vector<cCommand_MoveTo> g_vecAnimationCommands;

void cLuaBrain::ChangeTaylorSwiftTexture(std::string newTexture)
{
	//    pGroundMesh->friendlyName = "Ground";
	//    pGroundMesh->textureName[0] = "TaylorSwift_Eras_Poster.bmp";

	cMesh* pGround = cGameManager::GetGMInstance()->FindMeshByFriendlyName("Ground");
	if (pGround)
	{
		//        pGround->textureName[0] = "TaylorSwift_Eras_Poster.bmp";
		pGround->textureName[0] = newTexture;   // "SpidermanUV_square.bmp";
	}
	return;
}

int cLuaBrain::lua_ChangeTaylorSwiftTexture(lua_State* L)
{
	//    ChangeTaylorSwiftTexture("SpidermanUV_square.bmp");

		// Look up whatever was passed on the stack
		// https://www.lua.org/pil/24.2.2.html:
		// "The first element in the stack (that is, the element that was pushed first) 
		//    has index 1, the next one has index 2, and so on."

	const char* bitMapName = lua_tostring(L, 1);
	std::string sBitMapName(bitMapName);

	ChangeTaylorSwiftTexture(sBitMapName);

	return 0;
}

cLuaBrain::cLuaBrain()
{
	//	this->m_p_vecGOs = nullptr;

		// Create new Lua state.
		// NOTE: this is common to ALL script in this case
	this->m_pLuaState = luaL_newstate();

	luaL_openlibs(this->m_pLuaState);					/* Lua 5.3.3 */


	//	lua_pushcfunction( this->m_pLuaState, cLuaBrain::l_UpdateObject );
	//	lua_setglobal( this->m_pLuaState, "setObjectState" );

	//	lua_pushcfunction( this->m_pLuaState, cLuaBrain::l_GetObjectState );
	//	lua_setglobal( this->m_pLuaState, "getObjectState" );

		// Connecting the static function here with the function that
		//	we call with the Lua script
	//	lua_pushcfunction( this->m_pLuaState, cLuaBrain::l_ChangeTaylorSwiftTexture );

		// Pass Lua the pointer to the function.
		// MUST be known at compile time for this to work
	lua_pushcfunction(this->m_pLuaState, lua_ChangeTaylorSwiftTexture);
	lua_setglobal(this->m_pLuaState, "ChangeTaylorSwiftTexture");

	// Note: the names don't have to match. 
	// They have nothing to do with each other
	lua_pushcfunction(this->m_pLuaState, lua_AddSerialMoveObjectCommand);
	lua_setglobal(this->m_pLuaState, "AddMoveCommand");

	lua_pushcfunction(this->m_pLuaState, lua_SetMeshPositionByFriendlyName);
	lua_setglobal(this->m_pLuaState, "SetMeshPositionByFriendlyName");

	lua_pushcfunction(this->m_pLuaState, lua_GetMeshPositionByFriendlyName);
	lua_setglobal(this->m_pLuaState, "GetMeshPositionByFriendlyName");

	lua_pushcfunction(this->m_pLuaState, lua_SetMeshPositionByFriendlyName);
	lua_setglobal(this->m_pLuaState, "SetMeshPositionByFriendlyName");

	return;
}

cLuaBrain::~cLuaBrain()
{
	lua_close(this->m_pLuaState);
	return;
}


// Saves (and overwrites) any script
// scriptName is just so we can delete them later
void cLuaBrain::LoadScript(std::string scriptName,
	std::string scriptSource)
{
	this->m_mapScripts[scriptName] = scriptSource;
	return;
}


void cLuaBrain::DeleteScript(std::string scriptName)
{
	this->m_mapScripts.erase(scriptName);
	return;
}

// Passes a pointer to the game object vector
//void cLuaBrain::SetObjectVector( std::vector< cGameObject* >* p_vecGOs )
//{
//	this->m_p_vecGOs = p_vecGOs;
//	return;
//}

// Call all the active scripts that are loaded
void cLuaBrain::Update(float deltaTime)
{
	//	std::cout << "cLuaBrain::Update() called" << std::endl;
	for (std::map< std::string /*name*/, std::string /*source*/>::iterator itScript =
		this->m_mapScripts.begin(); itScript != this->m_mapScripts.end(); itScript++)
	{

		// Pass the script into Lua and update
//		int error = luaL_loadbuffer(L, buffer, strlen(buffer), "line");

		std::string curLuaScript = itScript->second;

		int error = luaL_loadstring(this->m_pLuaState,
			curLuaScript.c_str());

		if (error != 0 /*no error*/)
		{
			std::cout << "-------------------------------------------------------" << std::endl;
			std::cout << "Error running Lua script: ";
			std::cout << itScript->first << std::endl;
			std::cout << this->m_decodeLuaErrorToString(error) << std::endl;
			std::cout << "-------------------------------------------------------" << std::endl;
			continue;
		}

		// execute funtion in "protected mode", where problems are 
		//  caught and placed on the stack for investigation
		error = lua_pcall(this->m_pLuaState,	/* lua state */
			0,	/* nargs: number of arguments pushed onto the lua stack */
			0,	/* nresults: number of results that should be on stack at end*/
			0);	/* errfunc: location, in stack, of error function.
					 if 0, results are on top of stack. */
		if (error != 0 /*no error*/)
		{
			std::cout << "Lua: There was an error..." << std::endl;
			std::cout << this->m_decodeLuaErrorToString(error) << std::endl;

			std::string luaError;
			// Get error information from top of stack (-1 is top)
			luaError.append(lua_tostring(this->m_pLuaState, -1));

			// Make error message a little more clear
			std::cout << "-------------------------------------------------------" << std::endl;
			std::cout << "Error running Lua script: ";
			std::cout << itScript->first << std::endl;
			std::cout << luaError << std::endl;
			std::cout << "-------------------------------------------------------" << std::endl;
			// We passed zero (0) as errfunc, so error is on stack)
			lua_pop(this->m_pLuaState, 1);  /* pop error message from the stack */

			continue;
		}

	}

	return;
}

// Runs a script, but doesn't save it (originally used to set the ObjectID)
void cLuaBrain::RunScriptImmediately(std::string script)
{

	int error = luaL_loadstring(this->m_pLuaState,
		script.c_str());

	if (error != 0 /*no error*/)
	{
		std::cout << "-------------------------------------------------------" << std::endl;
		std::cout << "Error running Lua script: ";
		std::cout << this->m_decodeLuaErrorToString(error) << std::endl;
		std::cout << "-------------------------------------------------------" << std::endl;
		return;
	}

	// execute funtion in "protected mode", where problems are 
	//  caught and placed on the stack for investigation
	error = lua_pcall(this->m_pLuaState,	/* lua state */
		0,	/* nargs: number of arguments pushed onto the lua stack */
		0,	/* nresults: number of results that should be on stack at end*/
		0);	/* errfunc: location, in stack, of error function.
				if 0, results are on top of stack. */
	if (error != 0 /*no error*/)
	{
		std::cout << "Lua: There was an error..." << std::endl;
		std::cout << this->m_decodeLuaErrorToString(error) << std::endl;

		std::string luaError;
		// Get error information from top of stack (-1 is top)
		luaError.append(lua_tostring(this->m_pLuaState, -1));

		// Make error message a little more clear
		std::cout << "-------------------------------------------------------" << std::endl;
		std::cout << "Error running Lua script: ";
		std::cout << luaError << std::endl;
		std::cout << "-------------------------------------------------------" << std::endl;
		// We passed zero (0) as errfunc, so error is on stack)
		lua_pop(this->m_pLuaState, 1);  /* pop error message from the stack */
	}

	return;
}





int cLuaBrain::lua_AddSerialMoveObjectCommand(lua_State* L)
{
	// We have to decide what we are typing into Lua.
	// It's 100% up to us
	// 
	// How about:
	// AddMoveCommand('bathtub', Dx, Dy, Dz, timeInSeconds)
	//
	// The first string is the friendly name
	// The Dx, etc. is "Destination" X,Y,Z



	std::string MeshFriendlyName(lua_tostring(L, 1));       // 'bathtub'
	//cMesh* pBathTub = g_pFindMeshByFriendlyName(MeshFriendlyName);
	cMesh* pBathTub = cGameManager::GetGMInstance()->FindMeshByFriendlyName(MeshFriendlyName);


	// https://pgl.yoyo.org/luai/i/lua_Number
	// All Lua numbers are stored as doubles
	// typedef double lua_Number;

	glm::vec3 destinationXYZ = glm::vec3(0.0f);
	destinationXYZ.x = (float)lua_tonumber(L, 2);     // Dx (destination X)
	destinationXYZ.y = (float)lua_tonumber(L, 3);     // Dy (destination Y)
	destinationXYZ.z = (float)lua_tonumber(L, 4);     // Dz (destination Z)
	float timeInSeconds = (float)lua_tonumber(L, 5);   // timeInSeconds

	//    cCommand_MoveTo moveBathTub(pBathTub,
	//                                pBathTub->getDrawPosition(),
	//                                glm::vec3(50.0f, 0.0f, 0.0f),
	//                                10.0f);

	cCommand_MoveTo moveBathTub(pBathTub,
		pBathTub->getDrawPosition(),
		destinationXYZ,
		timeInSeconds);

	::g_vecAnimationCommands.push_back(moveBathTub);


	return 0;
}

int cLuaBrain::lua_GetMeshPositionByFriendlyName(lua_State* L)
{
	// Example Lua script might be:
	// 
	// bIsValid, x, y, z = GetMeshPositionByFriendlyName('bathtub');
	// 
	// 1st parameter indicates if we found this object or not
	// 
	// Returns the x, y, z location

	std::string MeshFriendlyName(lua_tostring(L, 1));       // 'bathtub'
	cMesh* pBathTub = cGameManager::GetGMInstance()->FindMeshByFriendlyName(MeshFriendlyName);

	if (pBathTub)
	{
		// 1st parameter: indicate if this object was found
		lua_pushboolean(L, true);
		lua_pushnumber(L, pBathTub->getDrawPosition().x);
		lua_pushnumber(L, pBathTub->getDrawPosition().y);
		lua_pushnumber(L, pBathTub->getDrawPosition().z);
		// Tell Lua how many things got pushed onto the stack
		return 4;
	}

	// Didn't find it
	lua_pushboolean(L, false);

	// We returned 1 value, a "false"
	return 1;
}


//static 
int l_ChangeTaylorSwiftTexture(lua_State* L)
{
	// return as a "c string" (pointer to a char array)
	// Note:  When a C function returns, Lua clears its stack; therefore, 
	//	as a rule, you should never store pointers to Lua strings outside 
	// the function that got them.
	const char* bitmapname = lua_tostring(L, 1);
	std::string sBitMapName(bitmapname);

	return 0;
}

// Called by Lua
// Passes object ID, new velocity, etc.
// Returns valid (true or false)
// Passes: 
// - position (xyz)
// - velocity (xyz)
// called "setObjectState" in lua
/*static*/
//int cLuaBrain::l_UpdateObject( lua_State *L )
//{
//	int objectID = (int)lua_tonumber(L, 1);	/* get argument */
//	
//	// Exist? 
//	cGameObject* pGO = cLuaBrain::m_findObjectByID(objectID);
//
//	if ( pGO == nullptr )
//	{	// No, it's invalid
//		lua_pushboolean( L,  false );
//		// I pushed 1 thing on stack, so return 1;
//		return 1;	
//	}
//
//	// Object ID is valid
//	// Get the values that lua pushed and update object
//	cPhysicalProperties physState;
//	pGO->GetPhysState(physState);
//
//	physState.position.x = (float)lua_tonumber(L, 2);	/* get argument */
//	physState.position.y = (float)lua_tonumber(L, 3);	/* get argument */
//	physState.position.z = (float)lua_tonumber(L, 4);	/* get argument */
//	physState.velocity.x = (float)lua_tonumber(L, 5);	/* get argument */
//	physState.velocity.y = (float)lua_tonumber(L, 6);	/* get argument */
//	physState.velocity.z = (float)lua_tonumber(L, 7);	/* get argument */
//
//	pGO->SetPhysState(physState);
//
//
//	lua_pushboolean( L, true );	// index is OK
//	
//	return 1;		// There were 7 things on the stack
//
//}

// Passes object ID
// Returns valid (true or false)
// - position (xyz)
// - velocity (xyz)
// called "getObjectState" in lua
/*static*/
//int cLuaBrain::l_GetObjectState( lua_State *L )
//{
//	int objectID = (int)lua_tonumber(L, 1);	/* get argument */
//	
//	// Exist? 
//	cGameObject* pGO = cLuaBrain::m_findObjectByID(objectID);
//
//	if ( pGO == nullptr )
//	{	// No, it's invalid
//		lua_pushboolean( L,  false );
//		// I pushed 1 thing on stack, so return 1;
//		return 1;	
//	}
//
//	// Object ID is valid
//	cPhysicalProperties physState;
//	pGO->GetPhysState(physState);
//
//	lua_pushboolean( L, true );	// index is OK
//	lua_pushnumber( L, physState.position.x );		
//	lua_pushnumber( L, physState.position.y );		
//	lua_pushnumber( L, physState.position.z );		
//	lua_pushnumber( L, physState.velocity.x );		
//	lua_pushnumber( L, physState.velocity.y );		
//	lua_pushnumber( L, physState.velocity.z );		
//	
//	return 7;		// There were 7 things on the stack
//}

/*static*/
//std::vector< cGameObject* >* cLuaBrain::m_p_vecGOs;


// returns nullptr if not found
/*static*/
//cGameObject* cLuaBrain::m_findObjectByID( int ID )
//{
//	for ( std::vector<cGameObject*>::iterator itGO = cLuaBrain::m_p_vecGOs->begin(); 
//		  itGO != cLuaBrain::m_p_vecGOs->end(); itGO++ )
//	{
//		if ( (*itGO)->getUniqueID() == ID )
//		{	// Found it!
//			return (*itGO);
//		}
//	}//for ( std::vector<cGameObject*>::iterator itGO...
//	// Didn't find it
//	return nullptr;
//}


int cLuaBrain::lua_SetMeshPositionByFriendlyName(lua_State* L)
{
	// Example Lua script might be
	// SetMeshPositionByFriendlyName('bathtub', x, y, z )

	std::string MeshFriendlyName(lua_tostring(L, 1));       // 'bathtub'
	cMesh* pBathTub = cGameManager::GetGMInstance()->FindMeshByFriendlyName(MeshFriendlyName);

	glm::vec3 newLocationXYZ = glm::vec3(0.0f);
	newLocationXYZ.x = (float)lua_tonumber(L, 2);     // Dx (destination X)
	newLocationXYZ.y = (float)lua_tonumber(L, 3);     // Dy (destination Y)
	newLocationXYZ.z = (float)lua_tonumber(L, 4);     // Dz (destination Z)

	pBathTub->drawPosition = newLocationXYZ;

	return 0;
}

std::string cLuaBrain::m_decodeLuaErrorToString(int error)
{
	switch (error)
	{
	case 0:
		return "Lua: no error";
		break;
	case LUA_ERRSYNTAX:
		return "Lua: syntax error";
		break;
	case LUA_ERRMEM:
		return "Lua: memory allocation error";
		break;
	case LUA_ERRRUN:
		return "Lua: Runtime error";
		break;
	case LUA_ERRERR:
		return "Lua: Error while running the error handler function";
		break;
	}//switch ( error )

	// Who knows what this error is?
	return "Lua: UNKNOWN error";
}
