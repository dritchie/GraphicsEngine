#pragma once

#ifndef GRAPHICS_ENGINE__SHADER_EXPRESSIONS_H
#define GRAPHICS_ENGINE__SHADER_EXPRESSIONS_H

#include "Assets/Shader/Shader.h"
#include "Assets/ShaderAsset.h"
#include "Eigen/Core"
#include <unordered_map>

namespace GraphicsEngine
{	
	////////////////////////////// EVAR DECLARATIONS ///////////////////////////////////////

	class Var : public ShaderAsset
	{
	public:
		virtual bool SameTypeAs(Var* other) = 0;
		const std::string& Name() const { return name; }

	protected:
		std::string name;
	};

	class AttribVar : public Var
	{
	public:
		AttribVar(const std::string& inName, Shader::ShaderStage& s);
		void Bind();
		void ShaderCode(ShaderAssetCode& code);
		bool SameTypeAs(Var* other);
	};

	class UniformFloatVar : public Var
	{
	public:
		UniformFloatVar(const std::string& n, Shader::ShaderStage s, float f = 0.0f);
		void Bind();
		void ShaderCode(ShaderAssetCode& code);
		bool SameTypeAs(Var* other);

		float val;
	};

	class UniformVec2Var : public Var
	{
	public:
		UniformVec2Var(const std::string& n, Shader::ShaderStage s,
			const Eigen::Vector2f& = Eigen::Vector2f(0.0f, 0.0f));
		void Bind();
		void ShaderCode(ShaderAssetCode& code);
		bool SameTypeAs(Var* other);

		Eigen::Vector2f val;
	};

	class UniformVec3Var : public Var
	{
	public:
		UniformVec3Var(const std::string& n, Shader::ShaderStage s,
			const Eigen::Vector2f& = Eigen::Vector3f(0.0f, 0.0f, 0.0f));
		void Bind();
		void ShaderCode(ShaderAssetCode& code);
		bool SameTypeAs(Var* other);

		Eigen::Vector3f val;
	};

	class UniformVec4Var : public Var
	{
	public:
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
		UniformVec4Var(const std::string& n, Shader::ShaderStage s,
			const Eigen::Vector2f& = Eigen::Vector3f(0.0f, 0.0f, 0.0f, 0.0f));
		void Bind();
		void ShaderCode(ShaderAssetCode& code);
		bool SameTypeAs(Var* other);

		Eigen::Vector4f val;
	};

	// TODO: Uniform texture var

	///////////////////////// EXPRESSION DECLARATION /////////////////////////////////

	// Expressions are templatized by a GLSL type
	enum GLSLType
	{
		_float_,
		_vec2_,
		_vec3_,
		_vec4_
	};
	std::string GLSLType_ToString(GLSLType t);	// TODO: Implement this

	typedef std::shared_ptr<Var> VarRef;

	template <GLSLType T>
	class Expression : public ShaderAsset
	{
	public:

		// Overloadable constructor for atomic expressions
		template<typename S> Expression(const S& val);

		// Adding and retrieving vars
		VarRef operator [] (const std::string& varName);
		bool AddVar(VarRef v);
		void InheritVars(const Expression<T>& other);

		// Inspecting expression text
		std::string RawText(const std::string& exprName) const;
		std::string WrappedText(const std::string& exprName) const;

		void ShaderCode(ShaderAssetCode& code);
		void Bind();

	private:
		std::string expr;
		std::unordered_map<std::string, VarRef> vars;
	};

	////////////////////////// OPERATOR DECLARATIONS /////////////////////////////////

	/** Expression constructors (meant to be called implicitly) **/

	template<>
	Expression<_float_>::Expression(const float& val)
	{
		char tmp[256];
		SafePrintf(tmp, "%f", val);
		expr = std::string(tmp);
	}

	template<>
	Expression<_vec2_>::Expression(const Eigen::Vector2f& val)
	{
		char tmp[256];
		SafePrintf(tmp, "vec2(%f,%f)", val.x(), val.y());
		expr = std::string(tmp);
	}

	template<>
	Expression<_vec3_>::Expression(const Eigen::Vector3f& val)
	{
		char tmp[256];
		SafePrintf(tmp, "vec3(%f,%f,%f)", val.x(), val.y(), val.z());
		expr = std::string(tmp);
	}

	template<>
	Expression<_vec4_>::Expression(const Eigen::Vector4f& val)
	{
		char tmp[256];
		SafePrintf(tmp, "vec3(%f,%f,%f,%f)", val.x(), val.y(), val.z(), val.w());
		expr = std::string(tmp);
	}

	template<GLSLType T>
	Expression<T>::Expression(const std::string& text)
	{
		expr = text;
	}
	
	/** Arithmetic operators **/

	std::string ParenWrap(const std::string& s)
	{
		return std::string("(") + s + (")");
	}

	template<GLSLType S, GLSLType T>
	Expression<T> BinaryOp(const Expression<S>& left, const Expression<T>& right, const std::string& op)
	{
		Expression<T> retVal(ParenWrap(left.RawText()) + " " + op + " " + ParenWrap(right.RawText()));
		retVal.InheritVars(left);
		retVal.InheritVars(right);
		return retVal;
	}

	template<GLSLType T>
	Expression<T> operator + (const Expression<T>& left, const Expression<T>& right)
	{
		return BinaryOp(left, right, "+");
	}

	template<GLSLType T>
	Expression<T> operator - (const Expression<T>& left, const Expression<T>& right)
	{
		return BinaryOp(left, right, "-");
	}

	template<GLSLType T>
	Expression<T> operator * (const Expression<T>& left, const Expression<T>& right)
	{
		return BinaryOp(left, right, "*");
	}

	template<GLSLType T>
	Expression<T> operator / (const Expression<T>& left, const Expression<T>& right)
	{
		return BinaryOp(left, right, "/");
	}

	template<GLSLType T>
	Expression<T> operator * (const Expression<_float_>& left, const Expression<T>& right)
	{
		return BinaryOp(left, right, "*");
	}

	template<GLSLType T>
	Expression<T> operator * (const Expression<T>& left, const Expression<_float_>& right)
	{
		return BinaryOp(right, left, "*");
	}

	/** Type coercion **/

	template<GLSLType S, GLSLType T>
	Expression<T> Coerce(const Expression<S>& expr);

	template<>
	Expression<_float_> Coerce(const Expression<_float_>& expr)
	{
		return expr;
	}

	template<GLSLType T>
	Expression<_float_> Coerce(const Expression<T>& expr)
	{
		Expression<_float_> retval(ParenWrap(expr.RawText) + ".x");
		retval.InheritVars(expr);
		return retval;
	}

	template<>
	Expression<_vec2_> Coerce(const Expression<_float_>& expr)
	{
		std::string text = ParenWrap(expr.RawText());
		Expression<_vec2_> retval(std::string("vec2(") + text + "," + text + ")");
		retval.InheritVars(expr);
		return retval;
	}

	template<>
	Expression<_vec2_> Coerce(const Expression<_vec2_>& expr)
	{
		return expr;
	}

	template<GLSLType T>
	Expression<_vec2_> Coerce(const Expression<T>& expr)
	{
		Expression<_float_> retval(ParenWrap(expr.RawText) + ".xy");
		retval.InheritVars(expr);
		return retval;
	}

	template<>
	Expression<_vec3_> Coerce(const Expression<_float_>& expr)
	{
		std::string text = ParenWrap(expr.RawText());
		Expression<_vec3_> retval(std::string("vec3(") + text + "," + text + "," + text + ")");
		retval.InheritVars(expr);
		return retval;
	}

	template<>
	Expression<_vec2_> Coerce(const Expression<_vec3_>& expr)
	{
		return expr;
	}

	template<GLSLType T>
	Expression<_vec3_> Coerce(const Expression<_vec4_>& expr)
	{
		Expression<_float_> retval(ParenWrap(expr.RawText) + ".xyz");
		retval.InheritVars(expr);
		return retval;
	}

	template<>
	Expression<_vec4_> Coerce(const Expression<_float_>& expr)
	{
		std::string text = ParenWrap(expr.RawText());
		Expression<_vec4_> retval(std::string("vec4(") + text + "," + text + "," + text + "," + text + ")");
		retval.InheritVars(expr);
		return retval;
	}

	template<>
	Expression<_vec4_> Coerce(const Expression<_vec4_>& expr)
	{
		return expr;
	}

	/** Variable declarations **/

	///////////////////////// EXPRESSION DEFINITIONS /////////////////////////////////

	template <GLSLType T>
	bool Expression<T>::AddVar(VarRef v)
	{
		// Add the variable if a variable with the same name doesn't already exist
		// in this expression.
		// If a variable with the same name and a different type already exists, then
		// throw an error.
		auto it = vars.find(v->Name());
		if (it != vars.end() && !it->second->SameTypeAs(v))
			FatalError(std::string("Expression::AddVar - Attempted to redeclare existing variable with different type"))
		else if (it != vars.end())
			return false;
		else
		{
			vars[v->Name()] = v;
			return true;
		}
	}

	template <GLSLType T>
	void Expression<T>::InheritVars(const Expression<T>& other)
	{
		for (auto it = other.vars.begin(); it != other.vars.end(); it++)
			AddVar(it->second);
	}

	template <GLSLType T>
	Var* Expression<T>::operator [] (const std::string& varName)
	{
		auto it = vars.find(varname);
		if (it != vars.end())
			return it->second;
		else
			return NULL;
	}

	template <GLSLType T>
	std::string Expression<T>::RawText(const std::string& exprName) const
	{
		return expr;
	}

	template <GLSLType T>
	std::string Expression<T>::WrappedText(const std::string& exprName) const
	{
		// The actual expression, wrapped in a function identified
		// by the expression's name
		return GLSLType_ToString(T) + " " + exprName + "()\n{\nreturn " + expr + ";\n}\n";
	}

	template <GLSLType T>
	void Expression<T>::ShaderCode(ShaderAssetCode& code)
	{
		for (auto it = vars.begin(); it != vars.end(); it++)
			it->second->ShaderAssetCode(code);
	}

	template <GLSLType T>
	void Expression<T>::Bind()
	{
		for (auto it = vars.begin(); it != vars.end(); it++)
			it->second->Bind();
	}
}

#endif