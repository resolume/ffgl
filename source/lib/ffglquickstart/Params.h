#pragma once
#include <vector>
#include <string>
#include <atomic>
#include "Utils.h"
#include "../ffgl/FFGL.h"

class Param
{
protected:
	float value = 0;
	std::string name;
	FFUInt32 type;

public:
	typedef std::shared_ptr< Param > Ptr;
	static Ptr create()
	{
		return create( "" );
	};
	static Ptr create( std::string name )
	{
		return create( name, 0 );
	};
	static Ptr create( std::string name, float value )
	{
		return create( name, FF_TYPE_STANDARD, value );
	};
	static Ptr create( std::string name, FFUInt32 type, float value )
	{
		return std::make_shared< Param >( name, type, value );
	};

	Param() :
		Param( "" )
	{
	}
	Param( std::string name ) :
		Param( name, 0 )
	{
	}
	Param( std::string name, float value ) :
		Param( name, FF_TYPE_STANDARD, value )
	{
	}
	Param( std::string name, FFUInt32 type, float value ) :
		name( name ),
		type( type ),
		value( value )
	{
	}

	virtual void setValue( float v )
	{
		value = v;
	}
	virtual float getValue()
	{
		return value;
	}
	std::string getName()
	{
		return name;
	}
	FFUInt32 getType()
	{
		return type;
	}
};

class ParamRange : public Param
{
public:
	struct Range
	{
		float min = 0.0f, max = 1.0f;
		Range(float min, float max) : min(min), max(max) {};
	};
	typedef std::shared_ptr< ParamRange > Ptr;

	static Ptr create( std::string name, float value, Range range )
	{
		return std::make_shared< ParamRange >( name, value, range );
	}

	static Ptr createInteger( std::string name, int value, Range range )
	{
		auto res = std::make_shared< ParamRange >( name, (float)value, range );
		res->type = FF_TYPE_INTEGER;
		return res;
	}

	ParamRange( std::string name, float value, Range range ) :
		Param( name, FF_TYPE_STANDARD),
		range( range )
	{
		setValue( utils::map( value, range.min, range.max, 0.0, 1.0 ) ); 
	}

	float getRealValue()
	{
		return utils::map( value, 0.0, 1.0, range.min, range.max );
	}

	Range getRange()
	{
		return range;
	}

protected:
	Range range;
};

class ParamOption : public Param
{
public:
	struct Option
	{
		std::string name;
		float value;
	};
	typedef std::shared_ptr< ParamOption > Ptr;
	typedef std::vector< Option > Options;
	Options options;

	static Ptr create( std::string name, Options options )
	{
		return create( name, options, 0 );
	}
	static Ptr create( std::string name, Options options, int defaultOption )
	{
		return std::make_shared< ParamOption >( name, options, defaultOption );
	}

	ParamOption( std::string name, Options options ) :
		ParamOption( name, options, 0 )
	{
	}
	ParamOption( std::string name, Options options, int defaultOption ) :
		Param( name, FF_TYPE_OPTION, (float)defaultOption ),
		options( options )
	{
		setValue( (float)defaultOption );
	}

	void setValue( float _value ) override
	{
		options.size() <= _value ? value = 0 : value = _value;
		currentOption                                = options[ (size_t)value ];
	}

	float getRealValue()
	{
		return currentOption.value;
	}

	bool isCurrentOption( std::string option )
	{
		return option.compare( currentOption.name ) == 0;
	}

private:
	Option currentOption;
};

class ParamEvent : public Param
{
public:
	typedef std::shared_ptr< ParamEvent > Ptr;

	static Ptr create( std::string name )
	{
		return std::make_shared< ParamEvent >( name );
	}
	ParamEvent( std::string name ) :
		Param( name, FF_TYPE_EVENT, 0 )
	{
	}
};

class ParamTrigger : public ParamEvent
{
public:
	typedef std::shared_ptr< ParamTrigger > Ptr;

	static Ptr create( std::string name )
	{
		return std::make_shared< ParamTrigger >( name );
	}

	ParamTrigger( std::string name ) :
		ParamEvent( name )
	{
	}

	void consume()
	{
		value = false;
	}
};

class ParamBool : public Param
{
public:
	typedef std::shared_ptr< ParamBool > Ptr;
	static Ptr create( std::string name )
	{
		return create( name, false );
	}
	static Ptr create( std::string name, bool defaultValue )
	{
		return std::make_shared< ParamBool >( name, defaultValue );
	}

	ParamBool( std::string name ) :
		ParamBool( name, false )
	{
	}
	ParamBool( std::string name, bool defaultValue ) :
		Param( name, FF_TYPE_BOOLEAN, defaultValue )
	{
	}
};

class ParamText : public Param
{
public:
	typedef std::shared_ptr< ParamText > Ptr;
	std::string text;

	static Ptr create( std::string name )
	{
		return create( name, "" );
	}
	static Ptr create( std::string name, std::string text )
	{
		return std::make_shared< ParamText >( name, text );
	}

	ParamText( std::string name ) :
		ParamText( name, "" )
	{
	}
	ParamText( std::string name, std::string text ) :
		Param( name, FF_TYPE_TEXT, 0 ),
		text( text )
	{
	}
};
