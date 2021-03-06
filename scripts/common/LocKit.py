# Contains utility classes for working with merged TSV localization files.

import os.path
import re

from .LocParser import *
from .TsvParser import *
from .XlsParser import *
from .FullTsvParser import *

class LocKit:

	@staticmethod
	def readLocFiles(path, language = "", silent = False):
		if not silent:
			print("")
			print("Checking for files...")
			print("- path: %s" % path)
		files = LocParser.getFileList(path, language, silent)
		if not silent:
			print("")
			print("Parsing of %d file(s)..." % len(files))
		locFiles = []
		for filename in files:
			locFile = LocParser.parse(filename, language)
			locFile.filename = locFile.filename.replace(path + "/", "", 1)
			locFiles.append(locFile)
			if not silent:
				print("  - %s  (%d entries)" % (locFile.filename, len(locFile.entries)))
		return locFiles

	@staticmethod
	def readFullLocFiles(path, baseLanguage, silent = False):
		allLocFiles = LocKit.readLocFiles(path, "", silent)
		# languages first for sorting purposes
		languages = []
		for locFile in allLocFiles:
			languages.append(locFile.language)
		languages = list(dict.fromkeys(languages)) # remove duplicates
		languages.sort()
		if not baseLanguage in languages:
			raise BaseException("Could not find base language: " + baseLanguage)
		languages.remove(baseLanguage)
		languages.insert(0, baseLanguage)
		# categorize by reference filenames first
		filenames = []
		for locFile in allLocFiles:
			filenames.append(locFile.getReferenceFilename())
		filenames = list(dict.fromkeys(filenames)) # remove duplicates
		filenames.sort()
		# categorize all loc files
		locFiles = {}
		for filename in filenames:
			locFiles[filename] = []
			for language in languages:
				found = False
				for locFile in allLocFiles:
					if locFile.language == language and locFile.getReferenceFilename() == filename:
						locFiles[filename].append(locFile)
						found = True
						break
				if not found:
					locFiles[filename].append(None)
		# create data
		fullLocFiles = []
		for filename in filenames:
			keys = []
			for locFile in locFiles[filename]:
				if locFile != None:
					for entry in locFile.entries:
						if not entry.key in keys:
							keys.append(entry.key)
			entries = []
			for key in keys:
				values = []
				comment = ""
				for locFile in locFiles[filename]:
					if locFile != None:
						entry = locFile.findEntry(key)
						if entry != None:
							values.append(entry.value)
							if locFile.language == baseLanguage:
								comment = entry.comment
					else:
						values.append("")
				entries.append(LocFullEntry(key, values, comment))
			fullLocFiles.append(LocFullFile(os.path.dirname(filename), os.path.basename(filename), languages, entries))
		return fullLocFiles

	@staticmethod
	def writeLocFiles(path, locFiles, silent = False):
		if not silent:
			print("")
			print("Writing output %d file(s)..." % len(locFiles))
		try:
			os.makedirs(path)
		except:
			pass
		for locFile in locFiles:
			if not silent:
				print("  - %s  (%d entries)" % (locFile.filename, len(locFile.entries)))
			text = LocParser.joinEntries(locFile.entries)
			try:
				os.makedirs(os.path.dirname(path + "/" + locFile.filename))
			except:
				pass
			file = open(path + "/" + locFile.filename, "wb")
			file.write(LocParser.BOM + text.encode())
			file.close()

	@staticmethod
	def writeLocFullFiles(path, locFullFiles, silent = False):
		if not silent:
			print("")
			print("Writing output %d file(s)..." % len(locFullFiles))
		try:
			os.makedirs(path)
		except:
			pass
		for locFullFile in locFullFiles:
			for language in locFullFile.languages:
				locFile = locFullFile.generateLocFile(language)
				filename = path + "/" + locFile.filename
				if not silent:
					print("  - %s  (%d entries)" % (filename, len(locFullFile.entries)))
				text = LocParser.joinEntries(locFile.entries)
				try:
					os.makedirs(os.path.dirname(filename))
				except:
					pass
				file = open(filename, "wb")
				file.write(LocParser.BOM + text.encode())
				file.close()

	@staticmethod
	def readTsvFile(inputFilename, silent = False):
		if not silent:
			print("")
			print("Parsing TSV file...")
		locFiles = TsvParser.parse(inputFilename)
		if not silent:
			for locFile in locFiles:
				print("  - %s  (%d entries)" % (locFile.filename, len(locFile.entries)))
		return locFiles

	@staticmethod
	def writeTsvFile(filename, locFiles, silent = False):
		if not silent:
			print("")
			print("Writing output file...")
			for locFile in locFiles:
				print("  - %s  (%d entries)" % (locFile.filename, len(locFile.entries)))
		text = TsvParser.generateFile(locFiles)
		file = open(filename, "wb")
		file.write(LocParser.BOM + text.encode())
		file.close()

	@staticmethod
	def readFullTsvFile(inputFilename, silent = False):
		if not silent:
			print("")
			print("Parsing Full-TSV file...")
		locFiles = FullTsvParser.parse(inputFilename)
		if not silent:
			for locFile in locFiles:
				print("  - %s  (%d entries)" % (locFile.filename, len(locFile.entries)))
		return locFiles

	@staticmethod
	def writeFullTsvFile(filename, locFiles, silent = False):
		if not silent:
			print("")
			print("Writing output file...")
			for locFile in locFiles:
				print("  - %s - %s  (%d entries)" % (locFile.path, locFile.filename, len(locFile.entries)))
		text = FullTsvParser.generateFile(locFiles)
		file = open(filename, "wb")
		file.write(LocParser.BOM + text.encode())
		file.close()

	@staticmethod
	def readXlsFile(inputFilename, silent = False):
		if not silent:
			print("")
			print("Parsing XLS file...")
		locFiles = XlsParser.parse(inputFilename)
		if not silent:
			for locFile in locFiles:
				print("  - %s  (%d entries)" % (locFile.filename, len(locFile.entries)))
		return locFiles

	@staticmethod
	def writeXlsFile(filename, locFiles, silent = False):
		if not silent:
			print("")
			print("Writing output file...")
			for locFile in locFiles:
				print("  - %s  (%d entries)" % (locFile.filename, len(locFile.entries)))
		wb = XlsParser.generateFile(locFiles)
		wb.save(filename)

	@staticmethod
	def _findLocFile(originalLocFiles, locFile, warning = True):
		for file in originalLocFiles:
			if file.getReferenceFilename() == locFile.getReferenceFilename():
				return file
		if warning:
			print("----")
			print("WARNING! No corresponding original file exists for '%s'." % locFile.filename)
			print("----")
		return None

	@staticmethod
	def _findLocFullFile(originalLocFiles, locFile, warning = True):
		for file in originalLocFiles:
			if file.path == locFile.path and file.filename == locFile.filename:
				return file
		if warning:
			print("----")
			print("WARNING! No corresponding original file exists for '%s/%s'." % (locFile.path, locFile.filename))
			print("----")
		return None

	@staticmethod
	def _findLocEntry(originalLocEntries, locEntry, warning = True):
		for entry in originalLocEntries:
			if entry.key == locEntry.key:
				return entry
		if warning:
			print("----")
			print("WARNING! Key '%s' exists in new file, but does not exist in original." % locEntry.key)
			print("----")
		return None

	@staticmethod
	def insertOriginalLocFiles(locFiles, originalLocFiles):
		for locFile in locFiles:
			originalLocFile = LocKit._findLocFile(originalLocFiles, locFile)
			if originalLocFile != None:
				for locEntry in locFile.entries:
					originalLocEntry = LocKit._findLocEntry(originalLocFile.entries, locEntry)
					if originalLocEntry != None:
						locEntry.original = originalLocEntry.value
		return locFiles

	@staticmethod
	def createDifferenceLocFiles(locFiles, originalLocFiles, language, changedKeys):
		result = []
		for originalLocFile in originalLocFiles:
			locFile = LocKit._findLocFile(locFiles, originalLocFile, False)
			entries = []
			if locFile != None:
				entries = locFile.entries
			filename = originalLocFile.getReferenceFilename()
			basename = os.path.basename(filename)
			filename = filename.replace("/" + basename, "")
			if language != "":
				filename += "/" + language
			filename += "/" + basename
			locFile = LocFile(filename, language, [])
			for originalLocEntry in originalLocFile.entries:
				locEntry = LocKit._findLocEntry(entries, originalLocEntry, False)
				if locEntry == None or locEntry.key in changedKeys:
					locFile.entries.append(LocEntry(originalLocEntry.key, "", originalLocEntry.value, originalLocEntry.comment))
			if len(locFile.entries) > 0:
				result.append(locFile)
		return result

	@staticmethod
	def updateLocFiles(originalLocFiles, locFiles, language):
		result = []
		for locFile in locFiles:
			originalLocFile = LocKit._findLocFile(originalLocFiles, locFile, False)
			if originalLocFile == None:
				filename = locFile.getReferenceFilename()
				basename = os.path.basename(filename)
				filename = filename.replace("/" + basename, "")
				if language != "":
					filename += "/" + language
				filename += "/" + basename
				originalLocFile = LocFile(filename, language, [])
			for locEntry in locFile.entries:
				originalLocEntry = LocKit._findLocEntry(originalLocFile.entries, locEntry, False)
				if originalLocEntry == None:
					originalLocEntry = LocEntry(locEntry.key, locEntry.value, locEntry.original, locEntry.comment)
					originalLocFile.entries.append(originalLocEntry)
				originalLocEntry.value = locEntry.value
			if len(originalLocFile.entries):
				result.append(originalLocFile)
		return result

	@staticmethod
	def updateLocFullFiles(originalLocFiles, locFiles, baseLanguage):
		result = []
		newLocFiles = []
		allLanguages = []
		for locFile in locFiles:
			originalLocFile = LocKit._findLocFullFile(originalLocFiles, locFile, False)
			if originalLocFile != None:
				addedLanguages = 0
				for language in locFile.languages:
					if not language in originalLocFile.languages:
						originalLocFile.languages.append(language)
						addedLanguages = addedLanguages + 1
				allLanguages.extend(originalLocFile.languages)
				for entry in originalLocFile.entries:
					for i in range(addedLanguages):
						entry.values.append("")
				for locEntry in locFile.entries:
					originalLocEntry = LocKit._findLocEntry(originalLocFile.entries, locEntry, False)
					if originalLocEntry == None:
						emptyEntries = []
						for language in originalLocFile.languages:
							emptyEntries.append("")
						originalLocEntry = LocFullEntry(locEntry.key, emptyEntries, locEntry.comment)
						originalLocFile.entries.append(originalLocEntry)
					for i in range(len(locFile.languages)):
						index = originalLocFile.languages.index(locFile.languages[i])
						originalLocEntry.values[index] = locEntry.values[i]
				result.append(originalLocFile)
			else:
				newLocFiles.append(locFile)
				result.append(locFile)
		languages = []
		[languages.append(x) for x in allLanguages if x not in languages] # remove duplicates
		for locFile in newLocFiles:
			addedLanguages = 0
			for language in languages:
				if not language in locFile.languages:
					locFile.languages.append(language)
					addedLanguages = addedLanguages + 1
			for entry in locFile.entries:
				for i in range(addedLanguages):
					entry.values.append("")
		return result

	@staticmethod
	def renameKeys(locFiles, renamedKeys):
		for locFile in locFiles:
			print("RENAME " + locFile.filename)
			for locEntry in locFile.entries:
				print("   KEY " + locEntry.key)
				print(renamedKeys)
				if renamedKeys.has_key(locEntry.key):
					locEntry.key = renamedKeys[locEntry.key]
					print("RENAMED")
					break
		return locFiles
