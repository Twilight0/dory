/* -*- Mode: C; indent-tabs-mode: f; c-basic-offset: 4; tab-width: 4 -*- */

/* dory-global-preferences.h - Dory specific preference keys and
                                   functions.

   Copyright (C) 1999, 2000, 2001 Eazel, Inc.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with this program; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 51 Franklin Street - Suite 500,
   Boston, MA 02110-1335, USA.

   Authors: Ramiro Estrugo <ramiro@eazel.com>
*/

#ifndef DORY_GLOBAL_PREFERENCES_H
#define DORY_GLOBAL_PREFERENCES_H

#include <gio/gio.h>

G_BEGIN_DECLS

/* Trash options */
#define DORY_PREFERENCES_CONFIRM_MOVE_TO_TRASH	"confirm-move-to-trash"
#define DORY_PREFERENCES_CONFIRM_TRASH			"confirm-trash"
#define DORY_PREFERENCES_ENABLE_DELETE			"enable-delete"
#define DORY_PREFERENCES_SWAP_TRASH_DELETE      "swap-trash-delete"

/* Desktop options */
#define DORY_PREFERENCES_DESKTOP_IS_HOME_DIR                "desktop-is-home-dir"

/* Display  */
#define DORY_PREFERENCES_SHOW_HIDDEN_FILES			"show-hidden-files"
#define DORY_PREFERENCES_SHOW_ADVANCED_PERMISSIONS		"show-advanced-permissions"
#define DORY_PREFERENCES_DATE_FORMAT            "date-format"
#define DORY_PREFERENCES_DATE_FONT_CHOICE  "date-font-choice"
#define DORY_PREFERENCES_MONO_FONT_NAME "monospace-font-name"

/* Mouse */
#define DORY_PREFERENCES_MOUSE_USE_EXTRA_BUTTONS		"mouse-use-extra-buttons"
#define DORY_PREFERENCES_MOUSE_FORWARD_BUTTON		"mouse-forward-button"
#define DORY_PREFERENCES_MOUSE_BACK_BUTTON			"mouse-back-button"

typedef enum
{
	DORY_DATE_FORMAT_LOCALE,
	DORY_DATE_FORMAT_ISO,
	DORY_DATE_FORMAT_INFORMAL
} DoryDateFormat;

typedef enum
{
    DORY_DATE_FONT_CHOICE_AUTO,
    DORY_DATE_FONT_CHOICE_SYSTEM,
    DORY_DATE_FONT_CHOICE_NONE
} DoryDateFontChoice;

typedef enum
{
	DORY_NEW_TAB_POSITION_AFTER_CURRENT_TAB,
	DORY_NEW_TAB_POSITION_END,
} DoryNewTabPosition;

/* Sidebar panels  */
#define DORY_PREFERENCES_TREE_SHOW_ONLY_DIRECTORIES         "show-only-directories"

/* Single/Double click preference  */
#define DORY_PREFERENCES_CLICK_POLICY			"click-policy"

/* Quick renames with two single clicks and pause in-between*/
#define DORY_PREFERENCES_CLICK_TO_RENAME "quick-renames-with-pause-in-between"

/* Activating executable text files */
#define DORY_PREFERENCES_EXECUTABLE_TEXT_ACTIVATION		"executable-text-activation"

/* Image viewers to pass dory view sort order to */
#define DORY_PREFERENCES_IMAGE_VIEWERS_WITH_EXTERNAL_SORT "image-viewers-with-external-sort"

/* Spatial or browser mode */
#define DORY_PREFERENCES_ALWAYS_USE_BROWSER			"always-use-browser"
#define DORY_PREFERENCES_NEW_TAB_POSITION			"tabs-open-position"

#define DORY_PREFERENCES_SHOW_LOCATION_ENTRY		"show-location-entry"
#define DORY_PREFERENCES_SHOW_PREVIOUS_ICON_TOOLBAR     "show-previous-icon-toolbar"
#define DORY_PREFERENCES_SHOW_NEXT_ICON_TOOLBAR     "show-next-icon-toolbar"
#define DORY_PREFERENCES_SHOW_UP_ICON_TOOLBAR		"show-up-icon-toolbar"
#define DORY_PREFERENCES_SHOW_EDIT_ICON_TOOLBAR		"show-edit-icon-toolbar"
#define DORY_PREFERENCES_SHOW_RELOAD_ICON_TOOLBAR		"show-reload-icon-toolbar"
#define DORY_PREFERENCES_SHOW_HOME_ICON_TOOLBAR		"show-home-icon-toolbar"
#define DORY_PREFERENCES_SHOW_COMPUTER_ICON_TOOLBAR		"show-computer-icon-toolbar"
#define DORY_PREFERENCES_SHOW_SEARCH_ICON_TOOLBAR		"show-search-icon-toolbar"
#define DORY_PREFERENCES_SHOW_NEW_FOLDER_ICON_TOOLBAR   "show-new-folder-icon-toolbar"
#define DORY_PREFERENCES_SHOW_OPEN_IN_TERMINAL_TOOLBAR   "show-open-in-terminal-toolbar"
#define DORY_PREFERENCES_SHOW_ICON_VIEW_ICON_TOOLBAR   "show-icon-view-icon-toolbar"
#define DORY_PREFERENCES_SHOW_LIST_VIEW_ICON_TOOLBAR   "show-list-view-icon-toolbar"
#define DORY_PREFERENCES_SHOW_COMPACT_VIEW_ICON_TOOLBAR   "show-compact-view-icon-toolbar"
#define DORY_PREFERENCES_SHOW_ROOT_WARNING                "show-root-warning"
#define DORY_PREFERENCES_SHOW_SHOW_THUMBNAILS_TOOLBAR     "show-show-thumbnails-toolbar"
#define DORY_PREFERENCES_SHOW_TOGGLE_EXTRA_PANE_TOOLBAR "show-toggle-extra-pane-toolbar"

/* Which views should be displayed for new windows */
#define DORY_WINDOW_STATE_START_WITH_STATUS_BAR		"start-with-status-bar"
#define DORY_WINDOW_STATE_START_WITH_SIDEBAR		"start-with-sidebar"
#define DORY_WINDOW_STATE_START_WITH_TOOLBAR		"start-with-toolbar"
#define DORY_WINDOW_STATE_START_WITH_MENU_BAR           "start-with-menu-bar"
#define DORY_WINDOW_STATE_SIDE_PANE_VIEW                    "side-pane-view"
#define DORY_WINDOW_STATE_GEOMETRY				"geometry"
#define DORY_WINDOW_STATE_MAXIMIZED				"maximized"
#define DORY_WINDOW_STATE_SIDEBAR_WIDTH			"sidebar-width"
#define DORY_WINDOW_STATE_MY_COMPUTER_EXPANDED  "my-computer-expanded"
#define DORY_WINDOW_STATE_BOOKMARKS_EXPANDED    "bookmarks-expanded"
#define DORY_WINDOW_STATE_DEVICES_EXPANDED      "devices-expanded"
#define DORY_WINDOW_STATE_NETWORK_EXPANDED      "network-expanded"

/* Saved session (last closed window) */
#define DORY_WINDOW_STATE_SAVED_SPLIT_VIEW      "saved-split-view"
#define DORY_WINDOW_STATE_SAVED_TABS_LEFT       "saved-tabs-left"
#define DORY_WINDOW_STATE_SAVED_TABS_RIGHT      "saved-tabs-right"
#define DORY_WINDOW_STATE_SAVED_ACTIVE_TAB_LEFT "saved-active-tab-left"
#define DORY_WINDOW_STATE_SAVED_ACTIVE_TAB_RIGHT "saved-active-tab-right"

/* Sorting order */
#define DORY_PREFERENCES_SORT_DIRECTORIES_FIRST		"sort-directories-first"
#define DORY_PREFERENCES_SORT_FAVORITES_FIRST		"sort-favorites-first"
#define DORY_PREFERENCES_DEFAULT_SORT_ORDER			"default-sort-order"
#define DORY_PREFERENCES_DEFAULT_SORT_IN_REVERSE_ORDER	"default-sort-in-reverse-order"

/* The default folder viewer - one of the two enums below */
#define DORY_PREFERENCES_DEFAULT_FOLDER_VIEWER		"default-folder-viewer"
#define DORY_PREFERENCES_INHERIT_FOLDER_VIEWER		"inherit-folder-viewer"

#define DORY_PREFERENCES_SHOW_FULL_PATH_TITLES      "show-full-path-titles"

#define DORY_PREFERENCES_CLOSE_DEVICE_VIEW_ON_EJECT "close-device-view-on-device-eject"

#define DORY_PREFERENCES_START_WITH_DUAL_PANE "start-with-dual-pane"
#define DORY_PREFERENCES_RESTORE_TABS_ON_STARTUP "restore-tabs-on-startup"
#define DORY_PREFERENCES_IGNORE_VIEW_METADATA "ignore-view-metadata"
#define DORY_PREFERENCES_SHOW_BOOKMARKS_IN_TO_MENUS "show-bookmarks-in-to-menus"
#define DORY_PREFERENCES_SHOW_PLACES_IN_TO_MENUS "show-places-in-to-menus"

#define DORY_PREFERENCES_RECENT_ENABLED "remember-recent-files"

#define DORY_PREFERENCES_SIDEBAR_BOOKMARK_BREAKPOINT "sidebar-bookmark-breakpoint"

enum
{
	DORY_DEFAULT_FOLDER_VIEWER_ICON_VIEW,
	DORY_DEFAULT_FOLDER_VIEWER_COMPACT_VIEW,
	DORY_DEFAULT_FOLDER_VIEWER_LIST_VIEW,
	DORY_DEFAULT_FOLDER_VIEWER_OTHER
};

/* These IIDs are used by the preferences code and in dory-application.c */
#define DORY_ICON_VIEW_IID		"OAFIID:Dory_File_Manager_Icon_View"
#define DORY_COMPACT_VIEW_IID	"OAFIID:Dory_File_Manager_Compact_View"
#define DORY_LIST_VIEW_IID		"OAFIID:Dory_File_Manager_List_View"
#define DORY_DESKTOP_ICON_VIEW_IID  "OAFIID:Dory_File_Manager_Desktop_Icon_View"
#define DORY_DESKTOP_ICON_GRID_VIEW_IID  "OAFIID:Dory_File_Manager_Desktop_Icon_Grid_View"

/* Icon View */
#define DORY_PREFERENCES_ICON_VIEW_DEFAULT_ZOOM_LEVEL		"default-zoom-level"
#define DORY_PREFERENCES_ICON_VIEW_LABELS_BESIDE_ICONS		"labels-beside-icons"

/* Which text attributes appear beneath icon names */
#define DORY_PREFERENCES_ICON_VIEW_CAPTIONS				"captions"

/* The default size for thumbnail icons */
#define DORY_PREFERENCES_ICON_VIEW_THUMBNAIL_SIZE			"thumbnail-size"

/* ellipsization preferences */
#define DORY_PREFERENCES_ICON_VIEW_TEXT_ELLIPSIS_LIMIT		"text-ellipsis-limit"
#define DORY_PREFERENCES_DESKTOP_TEXT_ELLIPSIS_LIMIT		"text-ellipsis-limit"

/* Compact View */
#define DORY_PREFERENCES_COMPACT_VIEW_DEFAULT_ZOOM_LEVEL		"default-zoom-level"
#define DORY_PREFERENCES_COMPACT_VIEW_ALL_COLUMNS_SAME_WIDTH	"all-columns-have-same-width"

/* List View */
#define DORY_PREFERENCES_LIST_VIEW_DEFAULT_ZOOM_LEVEL		"default-zoom-level"
#define DORY_PREFERENCES_LIST_VIEW_DEFAULT_VISIBLE_COLUMNS		"default-visible-columns"
#define DORY_PREFERENCES_LIST_VIEW_DEFAULT_COLUMN_ORDER		"default-column-order"
#define DORY_PREFERENCES_LIST_VIEW_ENABLE_EXPANSION         "enable-folder-expansion"

#define DORY_PREFERENCES_MAX_THUMBNAIL_THREADS "thumbnail-threads"

enum
{
	DORY_CLICK_POLICY_SINGLE,
	DORY_CLICK_POLICY_DOUBLE
};

enum
{
	DORY_EXECUTABLE_TEXT_LAUNCH,
	DORY_EXECUTABLE_TEXT_DISPLAY,
	DORY_EXECUTABLE_TEXT_ASK
};

typedef enum
{
	DORY_SPEED_TRADEOFF_ALWAYS,
	DORY_SPEED_TRADEOFF_LOCAL_ONLY,
    DORY_SPEED_TRADEOFF_NEVER
} DorySpeedTradeoffValue;

#define DORY_PREFERENCES_SHOW_DIRECTORY_ITEM_COUNTS "show-directory-item-counts"
#define DORY_PREFERENCES_SHOW_IMAGE_FILE_THUMBNAILS	"show-image-thumbnails"
#define DORY_PREFERENCES_IMAGE_FILE_THUMBNAIL_LIMIT	"thumbnail-limit"
#define DORY_PREFERENCES_INHERIT_SHOW_THUMBNAILS "inherit-show-thumbnails"

#define DORY_PREFERENCES_DESKTOP_FONT		   "font"
#define DORY_PREFERENCES_DESKTOP_HOME_VISIBLE          "home-icon-visible"
#define DORY_PREFERENCES_DESKTOP_COMPUTER_VISIBLE      "computer-icon-visible"
#define DORY_PREFERENCES_DESKTOP_TRASH_VISIBLE         "trash-icon-visible"
#define DORY_PREFERENCES_DESKTOP_VOLUMES_VISIBLE	   "volumes-visible"
#define DORY_PREFERENCES_DESKTOP_NETWORK_VISIBLE       "network-icon-visible"
#define DORY_PREFERENCES_DESKTOP_BACKGROUND_FADE       "background-fade"
#define DORY_PREFERENCES_DESKTOP_IGNORED_DESKTOP_HANDLERS "ignored-desktop-handlers"

/* bulk rename utility */
#define DORY_PREFERENCES_BULK_RENAME_TOOL              "bulk-rename-tool"

/* Lockdown */
#define DORY_PREFERENCES_LOCKDOWN_COMMAND_LINE         "disable-command-line"

/* Desktop background */
#define DORY_PREFERENCES_DESKTOP_LAYOUT "desktop-layout"
#define DORY_PREFERENCES_SHOW_ORPHANED_DESKTOP_ICONS "show-orphaned-desktop-icons"
#define DORY_PREFERENCES_SHOW_DESKTOP   "show-desktop-icons"    /* DEPRECATED */
#define DORY_PREFERENCES_USE_DESKTOP_GRID "use-desktop-grid"
#define DORY_PREFERENCES_DESKTOP_HORIZONTAL_GRID_ADJUST "horizontal-grid-adjust"
#define DORY_PREFERENCES_DESKTOP_VERTICAL_GRID_ADJUST "vertical-grid-adjust"

/* File size unit prefix */
#define DORY_PREFERENCES_SIZE_PREFIXES			"size-prefixes"

/* media handling */

#define GNOME_DESKTOP_MEDIA_HANDLING_AUTOMOUNT            "automount"
#define GNOME_DESKTOP_MEDIA_HANDLING_AUTOMOUNT_OPEN       "automount-open"
#define GNOME_DESKTOP_MEDIA_HANDLING_AUTORUN              "autorun-never"
#define DORY_PREFERENCES_MEDIA_HANDLING_DETECT_CONTENT    "detect-content"

/* Terminal */
#define GNOME_DESKTOP_TERMINAL_EXEC        "exec"

/* Tooltips */
#define DORY_PREFERENCES_TOOLTIPS_DESKTOP              "tooltips-on-desktop"
#define DORY_PREFERENCES_TOOLTIPS_ICON_VIEW            "tooltips-in-icon-view"
#define DORY_PREFERENCES_TOOLTIPS_LIST_VIEW            "tooltips-in-list-view"
#define DORY_PREFERENCES_TOOLTIP_FILE_TYPE             "tooltips-show-file-type"
#define DORY_PREFERENCES_TOOLTIP_MOD_DATE              "tooltips-show-mod-date"
#define DORY_PREFERENCES_TOOLTIP_ACCESS_DATE           "tooltips-show-access-date"
#define DORY_PREFERENCES_TOOLTIP_CREATED_DATE          "tooltips-show-birth-date"
#define DORY_PREFERENCES_TOOLTIP_FULL_PATH             "tooltips-show-path"

#define DORY_PREFERENCES_DISABLE_MENU_WARNING          "disable-menu-warning"

/* Plugins */
#define DORY_PLUGIN_PREFERENCES_DISABLED_EXTENSIONS    "disabled-extensions"
#define DORY_PLUGIN_PREFERENCES_DISABLED_ACTIONS       "disabled-actions"
#define DORY_PLUGIN_PREFERENCES_DISABLED_SCRIPTS       "disabled-scripts"
#define DORY_PLUGIN_PREFERENCES_DISABLED_SEARCH_HELPERS "disabled-search-helpers"

/* Connect-to server dialog last-used method */
#define DORY_PREFERENCES_LAST_SERVER_CONNECT_METHOD "last-server-connect-method"

/* File operations queue */
#define DORY_PREFERENCES_NEVER_QUEUE_FILE_OPS          "never-queue-file-ops"

#define DORY_PREFERENCES_CLICK_DOUBLE_PARENT_FOLDER    "click-double-parent-folder"
#define DORY_PREFERENCES_EXPAND_ROW_ON_DND_DWELL       "expand-row-on-dnd-dwell"

#define DORY_PREFERENCES_SHOW_MIME_MAKE_EXECUTABLE     "enable-mime-actions-make-executable"
#define DORY_PREFERENCES_DEFERRED_ATTR_PRELOAD_LIMIT   "deferred-attribute-preload-limit"

#define DORY_PREFERENCES_SEARCH_CONTENT_REGEX          "search-content-use-regex"
#define DORY_PREFERENCES_SEARCH_FILES_REGEX            "search-files-use-regex"
#define DORY_PREFERENCES_SEARCH_REGEX_FORMAT           "search-regex-format"
#define DORY_PREFERENCES_SEARCH_USE_RAW                "search-content-use-raw"
#define DORY_PREFERENCES_SEARCH_FILE_CASE              "search-file-case-sensitive"
#define DORY_PREFERENCES_SEARCH_CONTENT_CASE           "search-content-case-sensitive"
#define DORY_PREFERENCES_SEARCH_SKIP_FOLDERS           "search-skip-folders"
#define DORY_PREFERENCES_SEARCH_FILES_RECURSIVELY      "search-files-recursively"
#define DORY_PREFERENCES_SEARCH_VISIBLE_COLUMNS        "search-visible-columns"
#define DORY_PREFERENCES_SEARCH_SORT_COLUMN            "search-sort-column"
#define DORY_PREFERENCES_SEARCH_REVERSE_SORT           "search-reverse-sort"

void dory_global_preferences_init                      (void);
void dory_global_preferences_finalize                  (void);
char *dory_global_preferences_get_default_folder_viewer_preference_as_iid (void);
gboolean dory_global_preferences_get_inherit_folder_viewer_preference (void);
gboolean dory_global_preferences_get_inherit_show_thumbnails_preference (void);
gboolean dory_global_preferences_get_ignore_view_metadata (void);
int dory_global_preferences_get_size_prefix_preference (void);
char *dory_global_preferences_get_desktop_iid (void);
gint dory_global_preferences_get_tooltip_flags (void);
gboolean dory_global_preferences_should_load_plugin (const gchar *name, const gchar *key);
gchar **dory_global_preferences_get_fileroller_mimetypes (void);

gchar *dory_global_preferences_get_mono_system_font (void);
gchar *dory_global_preferences_get_mono_font_family_match (const gchar *in_family);

extern GSettings *dory_preferences;
extern GSettings *dory_icon_view_preferences;
extern GSettings *dory_list_view_preferences;
extern GSettings *dory_compact_view_preferences;
extern GSettings *dory_desktop_preferences;
extern GSettings *dory_tree_sidebar_preferences;
extern GSettings *dory_window_state;
extern GSettings *gtk_filechooser_preferences;
extern GSettings *dory_plugin_preferences;
extern GSettings *dory_menu_config_preferences;
extern GSettings *dory_search_preferences;
extern GSettings *gnome_lockdown_preferences;
extern GSettings *gnome_background_preferences;
extern GSettings *gnome_media_handling_preferences;
extern GSettings *gnome_terminal_preferences;
extern GSettings *cinnamon_privacy_preferences;
extern GSettings *cinnamon_interface_preferences;
extern GSettings *gnome_interface_preferences;

/* Cached for fast access and used in dory-file.c for constructing date/time strings */
extern GTimeZone      *prefs_current_timezone;
extern gboolean        prefs_current_24h_time_format;
extern DoryDateFormat  prefs_current_date_format;

extern GTimer    *dory_startup_timer;

G_END_DECLS

#endif /* DORY_GLOBAL_PREFERENCES_H */
