#pragma once

#ifndef GRAPHICS_ENGINE__SHADER_ASSET_H
#define GRAPHICS_ENGINE__SHADER_ASSET_H

#include "Common/Common.h"

namespace GraphicsEngine
{
	struct ShaderAssetCode
	{
		std::vector<std::string> vertCode;
		std::vector<std::string> geoCode;
		std::vector<std::string> tessControlCode;
		std::vector<std::string> tessEvalCode;
		std::vector<std::string> fragCode;
	};

	/**
	Base class for all assets that contribute code to a shader program
	**/
	class ShaderAsset
	{
	public:

		// Sticks the code contributed by this asset into the 'code' struct.
		virtual void ShaderCode(ShaderAssetCode& code) = 0;

		// If this asset has changed a way that has changed its
		// code, then this method should return true to signal
		// the renderer that programs which depend on this asset should
		// be recompiled
		virtual bool CodeChanged() { return false; }

		// Bind uniforms needed by the shader program
		virtual void Bind() = 0;

		// Methods for downcasting
		template<class T> bool Is() { return dynamic_cast<T*>(this) != NULL; }
		template<class T> T* As() { return dynamic_cast<T*>(this); }

	protected:

		// Utilities for generating code at runtime
		static std::string Define(const std::string& key, const std::string& value);
		static std::string Block(const std::string& key, const std::vector<std::string>& lines);
		static std::string Block(const std::string& key, const std::string& contents);
	};
};


#endif