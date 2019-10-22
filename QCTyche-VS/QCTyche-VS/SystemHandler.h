#pragma once

#include <Windows.h>

#include "Status.h"
#include "ui.h"


/**
 *	Checks if the resolution of the main monitor is at least the requested size
 *
 *	@param ui_width				the width the monitor resolution must be at least
 *	@param ui_height			the height the monitor resolution must be at least
 *	@return						SUCCESS if the resolution matches the requirements otherwise an error
 */
TSTATUS checkResolutionRequirements(int ui_width, int ui_height) {
	// TODO: testen, ob es mehr als einen Bildschirm gibt, so wird nur der Hauptbildschirm überprüft!
	RECT screen;

	if (SystemParametersInfo(SPI_GETWORKAREA, 0, &screen, 0) != 0) {
		// Es gab einen Fehler beim einlesen der nutzbaren Bildschirmgroesse des Hauptbildschirms!
		return SH_GETWORKAREA;
	}

	if (screen.right < ui_width) {
		// Bildschirm zu klein (Breite) für die UI!
		return SH_SMALL_WIDTH;
	}

	if (screen.bottom < ui_height) {
		// Bildschirm zu klein (Höhe) für die UI, ggf im Vollbild mit Platz der Taskbar?

		if (GetSystemMetrics(SM_CYMAXIMIZED) < ui_height) {
			// Bildschirm zu klein (Höhe) ohne Taskbar
			return SH_SMALL_HEIGHT;
		} else {
			// Bildschirm zu klein (Höhe) mit Taskbar
			return SH_SMALL_HEIGHT_TB;
		}
	}

	return SUCCESS;
}