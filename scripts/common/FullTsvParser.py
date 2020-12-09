# Contains utility classes for working with merged Full-TSV localization files.

import os.path
import re

from .LocParser import *

class FullTsvParser:

	DELIMITER = "\t"
	DASH = "\xC2\xAD" # the "other" dash character in UTF-8

	@staticmethod
	def parse(filename):
		entries = []
		file = open(filename, "rb")
		# the first line of a UTF8 file can be "screwed up"
		skip = 0
		line = file.readline()
		for i in line:
			if i < 128:
				break
			skip += 1
		file.seek(skip, os.SEEK_SET)
		# read data
		string = file.read().decode().replace("\r", "")
		file.close()
		columnCount = len(string.split("\n")[0].split(FullTsvParser.DELIMITER))
		# regular expressions are awesome
		regex = ""
		# normal entry
		normal = "(?:[^\n]*?)"
		# special entry with quotes, escaped characters and \n support
		special = "\"(?:.|\n)*?\"?"
		# one entry is either normal or special
		entry = "(" + normal + "|" + special + ")?"
		# the regex is 3+ entries separated by delimiter characters and ending with \n
		regex += "(?:" + entry + FullTsvParser.DELIMITER + entry
		for i in range(columnCount - 2):
			regex += FullTsvParser.DELIMITER + entry
		regex += "\n)?"
		# now using that regex to extract all entries
		matches = re.findall(regex, string)
		# extracting LocFile instances
		languages = []
		locFiles = []
		locFile = None
		for match in matches:
			columns = list(match)
			columns.pop() # last column is for alignment
			for i in range(len(columns)):
				if columns[i].startswith("\"") and columns[i].endswith("\""):
					if "\n" in columns[i]:
						columns[i] = columns[i][1:-1]
					columns[i] = columns[i].replace("\"\"", "\"")
			if columns[0] == "###":
				languages = columns[2:len(columns)]
				while "" in languages:
					languages.remove("")
				locFile = LocFullFile(os.path.dirname(columns[1]), os.path.basename(columns[1]), languages, []) # cannot make [] a default argument because there's a bug in Python
				locFiles.append(locFile)
			elif columns[0] != "" and locFile != None:
				locFile.entries.append(LocFullEntry(columns[0], columns[2:len(languages) + 2], columns[1]))
		return locFiles
	
	@staticmethod
	def generateFile(locFiles):
		languages = []
		if len(locFiles) > 0:
			languages = locFiles[0].languages
		result = ""
		result += FullTsvParser._makeEntry("Key", "Comment", languages)
		for locFile in locFiles:
			result += "\n"
			result += FullTsvParser._makeEntry("###", locFile.getFullFilename(), languages)
			result += "\n"
			for locEntry in locFile.entries:
				result += FullTsvParser._makeEntry(locEntry.key, locEntry.comment, locEntry.values)
		return result
		
	@staticmethod
	def _makeEntry(key, comment, values):
		finalValues = []
		for value in values:
			finalValues.append(value.replace("\"", "\"\""))
		return '"%s"%s"%s"%s"%s"\n' % (key.replace("\"", "\"\""), FullTsvParser.DELIMITER, comment.replace("\"", "\"\""), FullTsvParser.DELIMITER,
			('"' + FullTsvParser.DELIMITER + '"').join(finalValues))
	
