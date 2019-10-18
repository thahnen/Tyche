#pragma once

#include <Windows.h>
#include "ui.h"


/**
 *	Checks if the resolution of the main monitor is at least the requested size
 *
 *	@param ui_width				the width the monitor resolution must be at least
 *	@param ui_height			the height the monitor resolution must be at least
 *	@return						0 if the resolution matches the requirements otherwise an error
 */
int checkResolutionRequirements(int ui_width, int ui_height) {
	// TODO: testen, ob es mehr als einen Bildschirm gibt, so wird nur der Hauptbildschirm überprüft!
	RECT screen;

	if (SystemParametersInfo(SPI_GETWORKAREA, 0, &screen, 0) != 0) {
		// Es gab einen Fehler beim einlesen der nutzbaren Bildschirmgroesse des Hauptbildschirms!
		return 1;
	}

	if (screen.right < ui_width) {
		// Bildschirm zu klein (Breite) für die UI!
		return 2;
	}

	if (screen.bottom < ui_height) {
		// Bildschirm zu klein (Höhe) für die UI, ggf im Vollbild mit Platz der Taskbar?

		if (GetSystemMetrics(SM_CYMAXIMIZED) < ui_height) {
			// Bildschirm zu klein (Höhe) ohne Taskbar
			return 4;
		} else {
			// Bildschirm zu klein (Höhe) mit Taskbar
			return 3;
		}
	}

	return 0;
}