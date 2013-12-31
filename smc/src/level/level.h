/***************************************************************************
 * level.h
 *
 * Copyright (C) 2003 - 2011 Florian Richter
 ***************************************************************************/
/*
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SMC_LEVEL_H
#define SMC_LEVEL_H

#include "../core/global_basic.h"
#include "../core/global_game.h"
#include "../level/level_background.h"
#include "../level/level_manager.h"
#include "../objects/level_entry.h"
#include "../audio/random_sound.h"
#include "../video/animation.h"
#include "../scripting/scripting.h"

namespace SMC
{

/* *** *** *** *** *** cLevel *** *** *** *** *** *** *** *** *** *** *** *** */

class cLevel : public CEGUI::XMLHandler
{
public:

#ifdef ENABLE_NEW_LOADER
	/// Loads a level from the given file.
	static cLevel* Load_From_File( boost::filesystem::path filename );
#endif

	cLevel( void );
	virtual ~cLevel( void );

	/* Create a new level with the given name (file extension and user
   * level directory are automatically added).
	 * returns true if successful
	*/
	bool New( std::string levelname );
	// Load an existing level by name (file extension and user or
  // game directory are automatically added).
	bool Load( std::string levelname );
	/* Unload the current Level
	 * if delayed is given unloads the on the next update
	*/
	void Unload( bool delayed = 0 );
#ifdef ENABLE_NEW_LOADER
	// Save the level to a file as XML
  boost::filesystem::path Save_To_File(boost::filesystem::path filename = boost::filesystem::path());
#endif
	// Save the Level
	void Save( void );
	// Delete and unload
	void Delete( void );
	// Reset settings data
	void Reset_Settings( void );

	// Initialize level elements on level creation/loading
	void Init( void );
	// Set this sprite manager active
	void Set_Sprite_Manager( void );
	// Enter level mode (not the level itself, see Init())
	void Enter( const GameMode old_mode = MODE_NOTHING );
	// Leave level mode (e.g. for showing the menu), level not necessarily unloaded
	void Leave( const GameMode next_mode = MODE_NOTHING );

	// Return the level’s name, derived from the filename.
	// If you don’t need the full absolute path of the level file,
	// use this method rather than accessing m_level_filename.
	std::string Get_Level_Name();

	// update level
	void Update( void );
	/* late level update
	 * needed for objects that need other objects to be already updated
	*/
	void Update_Late( void );

	// Draw Layer 1 ( Backgrounds, Level objects and Animations )
	void Draw_Layer_1( LevelDrawType type = LVL_DRAW );
	// Draw Layer 2 ( Global Effects )
	void Draw_Layer_2( LevelDrawType type = LVL_DRAW );

	// Function : Process_Input
	// static input handler
	void Process_Input( void );
	/* handle key down event
	 * returns true processed
	*/
	bool Key_Down( const SDLKey key );
	/* handle key up event
	 * returns true if processed
	*/
	bool Key_Up( const SDLKey key );
	/* handle mouse button down event
	 * returns true if processed
	*/
	bool Mouse_Down( Uint8 button );
	/* handle mouse button up event
	 * returns true if processed
	*/
	bool Mouse_Up( Uint8 button );
	/* handle joystick button down event
	 * returns true if processed
	*/
	bool Joy_Button_Down( Uint8 button );
	/* handle joystick button up event
	 * returns true if processed
	*/
	bool Joy_Button_Up( Uint8 button );

	/* Return the current Music filename relative to the music/
	 * directory.
	*/
	boost::filesystem::path Get_Music_Filename() const;
	// Set the Music filename. `filename' must be relative to
	// the music/ directory.
	void Set_Music( boost::filesystem::path filename );
	/* Set the filename
	 * rename_old : if set also rename the level file in the user folder
	*/
	void Set_Filename( boost::filesystem::path filename, bool rename_old = true );
	// Set the level author
	void Set_Author( const std::string &name );
	// Set the level version
	void Set_Version( const std::string &level_version );
	// Set the level description
	void Set_Description( const std::string &level_description );
	// Set the level difficulty ( 0 = undefined, 1 = dead easy and 100 = ultimate challenge )
	void Set_Difficulty( const Uint8 level_difficulty );
	// Set the level land type
	void Set_Land_Type( const LevelLandType level_land_type );

	// Get entry with the given name
	cLevel_Entry *Get_Entry( const std::string &name );

	// Return true if a level is loaded
	bool Is_Loaded( void ) const;

	static bool Is_Level_Object_Element( const CEGUI::String &element )
	{
		if( element == "information" || element == "settings" || element == "background" || element == "music" ||
			element == "global_effect" || element == "player" || element == "sound" || element == "particle_emitter" ||
			element == "path" || element == "sprite" || element == "powerup" || element == "item" || element == "enemy" ||
			element == "levelexit" || element == "level_entry" || element == "enemystopper" || element == "box" ||
			element == "moving_platform" || element == "falling_platform" || element == "ball" )
		{
			return 1;
		}

		return 0;
	};

	// level filename
	boost::filesystem::path m_level_filename;
	// if a new level should be loaded this is the next level filename
	boost::filesystem::path	 m_next_level_filename;
	// MRuby script code associated with this level
	std::string m_script;

	// unload the level on the next update
	bool m_delayed_unload;

	// background manager
	cBackground_Manager *m_background_manager;
	// animation manager
	cAnimation_Manager *m_animation_manager;
	// sprite manager
	cSprite_Manager *m_sprite_manager;
	// MRuby interpreter used for this level
	Scripting::cMRuby_Interpreter *m_mruby;

	/* *** *** *** Settings *** *** *** *** */

	// level engine version
	int m_engine_version;
	// last save time
	time_t m_last_saved;
	// author
	std::string m_author;
	// version
	std::string m_version;
	// music filename
	boost::filesystem::path m_musicfile;
	// valid music to play
	bool m_valid_music;
	// description
	std::string m_description;
	// difficulty ( 0 = undefined, 1 = dead easy and 100 = ultimate challenge )
	Uint8 m_difficulty;
	// land type
	LevelLandType m_land_type;

	// player
	float m_player_start_pos_x;
	float m_player_start_pos_y;
	ObjectDirection m_player_start_direction;
	// camera
	GL_rect m_camera_limits;
	float m_fixed_camera_hor_vel;

private:
	// XML element start (called by CEGUI on XML loading)
	virtual void elementStart( const CEGUI::String &element, const CEGUI::XMLAttributes &attributes );
	// XML element end (called by CEGUI on XML loading)
	virtual void elementEnd( const CEGUI::String &element );
	// XML text element (called by CEGUI on XML loading)
	virtual void text( const CEGUI::String &element );

	// XML element Item Tag list
	CEGUI::XMLAttributes m_xml_attributes;

	// XML: Indicates whether the <script> tag has been opened
	bool m_start_script_tag;
};

/* Return the Level Object if element name is available else NULL
 * engine_version : engine version of the data and if it's below the current version it converts it
 * sprite_manager : needed if the engine version is below the current version and data conversion creates multiple objects
*/
cSprite *Create_Level_Object_From_XML( const CEGUI::String &xml_element, CEGUI::XMLAttributes &attributes, int engine_version, cSprite_Manager *sprite_manager );

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */

// The Level
extern cLevel *pActive_Level;

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */

} // namespace SMC

#endif
