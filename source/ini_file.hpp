/**
 * Copyright (C) 2014 Patrick Mours. All rights reserved.
 * License: https://github.com/crosire/reshade#license
 */

#pragma once

#include <filesystem>
#include <unordered_map>
#include "variant.hpp"

namespace reshade
{
	class ini_file
	{
	public:
		explicit ini_file(const std::filesystem::path &path);
		explicit ini_file(const std::filesystem::path &path, const std::filesystem::path &save_path);
		~ini_file();

		template <typename T>
		void get(const std::string &section, const std::string &key, T &value) const
		{
			const auto it1 = _sections.find(section);

			if (it1 == _sections.end())
			{
				return;
			}

			const auto it2 = it1->second.find(key);

			if (it2 == it1->second.end())
			{
				return;
			}

			value = it2->second.as<T>();
		}
		void get(const std::string &section, const std::string &key, std::filesystem::path &value) const
		{
			const auto it1 = _sections.find(section);

			if (it1 == _sections.end())
			{
				return;
			}

			const auto it2 = it1->second.find(key);

			if (it2 == it1->second.end())
			{
				return;
			}

			value = std::filesystem::u8path(it2->second.as<std::string>());
		}
		template <typename T, size_t SIZE>
		void get(const std::string &section, const std::string &key, T(&values)[SIZE]) const
		{
			const auto it1 = _sections.find(section);

			if (it1 == _sections.end())
			{
				return;
			}

			const auto it2 = it1->second.find(key);

			if (it2 == it1->second.end())
			{
				return;
			}

			for (size_t i = 0; i < SIZE; i++)
			{
				values[i] = it2->second.as<T>(i);
			}
		}
		template <typename T>
		void get(const std::string &section, const std::string &key, std::vector<T> &values) const
		{
			const auto it1 = _sections.find(section);

			if (it1 == _sections.end())
			{
				return;
			}

			const auto it2 = it1->second.find(key);

			if (it2 == it1->second.end())
			{
				return;
			}

			values.clear();

			for (size_t i = 0; i < it2->second.data().size(); i++)
			{
				values.emplace_back(it2->second.as<T>(i));
			}
		}
		void get(const std::string &section, const std::string &key, std::vector<std::filesystem::path> &pathes) const
		{
			const auto it1 = _sections.find(section);

			if (it1 == _sections.end())
			{
				return;
			}

			const auto it2 = it1->second.find(key);

			if (it2 == it1->second.end())
			{
				return;
			}

			pathes.clear();

			for (size_t i = 0; i < it2->second.data().size(); i++)
			{
				pathes.emplace_back(std::filesystem::u8path(it2->second.as<std::string>(i)));
			}
		}
		template <typename T>
		void set(const std::string &section, const std::string &key, const T &value)
		{
			_modified = true;
			_sections[section][key] = value;
		}
		template <typename T, size_t SIZE>
		void set(const std::string &section, const std::string &key, const T(&values)[SIZE])
		{
			_modified = true;
			_sections[section][key] = values;
		}
		template <typename T, size_t SIZE>
		void set(const std::string &section, const std::string &key, const std::vector<T> &values)
		{
			_modified = true;
			_sections[section][key] = values;
		}
		void set(const std::string &section, const std::string &key, const std::vector<std::filesystem::path> &pathes)
		{
			_modified = true;
			std::vector<std::string> values;
			for (auto it = pathes.begin(); it != pathes.end(); it++)
			{
				values.push_back((*it).u8string());
			}
			_sections[section][key] = values;
		}

	private:
		void load();
		void save() const;

		bool _modified = false;
		std::filesystem::path _path;
		std::filesystem::path _save_path;
		using section = std::unordered_map<std::string, variant>;
		std::unordered_map<std::string, section> _sections;
	};
}
