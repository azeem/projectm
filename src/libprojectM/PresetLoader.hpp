#ifndef __PRESET_LOADER_HPP
#define __PRESET_LOADER_HPP

#include <string> // used for path / filename stuff

#include <memory> // for auto pointers
#include <sys/types.h>
#ifdef WIN32
#include "win32-dirent.h"
#endif

#ifdef LINUX
#include <dirent.h>
#endif

#ifdef MACOS
#include <dirent.h>
#endif

#include <vector>
#include <map>

class Preset;
class PresetInputs;
class PresetOutputs;
class PresetFactory;

class PresetLoader {
	public:
				
		/// Initializes the preset loader with the target directory specified 
		PresetLoader(std::string dirname);
				
		~PresetLoader();
	
		/// Load a preset by specifying a filename of the directory (that is, NOT full path) 	
		std::auto_ptr<Preset> loadPreset(unsigned int index) const;
		
		/// Associates a preset factory to a file extension.
		/// Any subsequent registrations of an extension overrides the previously set one
		/// \param extension the file extension to attach the handler to
		/// \param handler a preset loading handler that will be associated with the extension
		void registerFactory(const std::string & extension, PresetFactory * factory);

		/// Add a preset to the loader's collection.
		/// \param url an url referencing the preset
		/// \param presetName a name for the preset
		/// \param rating an integer representing the goodness of the preset
		/// \returns The unique index assigned to the preset in the collection. Used with loadPreset
		unsigned int addPresetURL ( const std::string & url, const std::string & presetName, int rating);
	
		
		/// Add a preset to the loader's collection.
		/// \param index insertion index
		/// \param url an url referencing the preset
		/// \param presetName a name for the preset
		/// \param rating an integer representing the goodness of the preset
		void insertPresetURL (unsigned int index, const std::string & url, const std::string & presetName, int rating);
	
		/// Clears all presets from the collection
		inline void clear() { _entries.clear(); _presetNames.clear(); _ratings.clear(); _ratingsSum = 0; }
		
		const std::vector<int> & getPresetRatings() const;
		int getPresetRatingsSum() const;
		
		void removePreset(unsigned int index);

		void setRating(unsigned int index, int rating);
		
		/// Get a preset rating given an index
		int getPresetRating ( unsigned int index) const;
		
		/// Get a preset url given an index
		const std::string & getPresetURL ( unsigned int index) const;
		
		/// Get a preset name given an index
		const std::string & getPresetName ( unsigned int index) const;
		
		/** Returns the number of presets in the active directory */
		inline std::size_t size() const {
			return _entries.size();
		}
					
		/** Sets the directory where the loader will search for files */	
		void setScanDirectory(std::string pathname);

		/// Returns the directory path associated with this preset chooser
		inline const std::string & directoryName() const {
			return _dirname;
		}

		/** Rescans the active preset directory */
		void rescan();

	private:
		void handleDirectoryError();
		std::string _dirname;
		DIR * _dir;
		mutable std::map<std::string, PresetFactory*> _factories;
		int _ratingsSum;
		
		// vector chosen for speed, but not great for reverse index lookups
		std::vector<std::string> _entries;
		std::vector<std::string> _presetNames;
		std::vector<int> _ratings;

};

#endif
