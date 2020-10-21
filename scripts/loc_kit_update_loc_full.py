# Localization LOC Updater

import os
import sys

from common.LocKit import *

VERSION = "2.3"

def process():
	print("-------------------------------------------------------------------------------")
	print("| AprilUI Localization LOC Updater " + VERSION)
	print("-------------------------------------------------------------------------------")
	
	if len(sys.argv) != 5:
		help()
		return
	path = sys.argv[1]
	language = ""
	inputFilename = sys.argv[2]
	originalPath = sys.argv[3]
	baseLanguage = sys.argv[4]
	
	originalLocFiles = LocKit.readFullLocFiles(originalPath, baseLanguage)
	newLocFiles = LocKit.readFullTsvFile(inputFilename)
	locFiles = LocKit.updateLocFullFiles(originalLocFiles, newLocFiles, baseLanguage)
	LocKit.writeLocFiles(path, locFiles)
	
def help():
	print("")
	print("Updates localization files from a split TSV file (as defined by AprilUI).")
	print("See examples/generic.loc for reference.")
	print("")
	print("usage:   loc_kit_update_loc.py PATH TSV_FILENAME ORG_PATH BASE_LANG")
	print("")
	print("  PATH          - Directory path for .loc file output.")
	print("  TSV_FILENAME  - Filename of the input TSV file.")
	print("  ORG_PATH      - Directory path to use as root recursive starting point.")
	print("  BASE_LANG     - Specific directory prefix for the base language files.")
	print("                  e.g. 'en' would make English the base language")
	print("")
	print("example: loc_kit_update_loc_full.py new_loc locale.txt ../data/localization/en en")
	print("")
	os.system("pause")

process()
