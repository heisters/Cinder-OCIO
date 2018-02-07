#pragma once

#include "cinder/gl/gl.h"
#include "glvideo.h"
#include "Movie.h"
#include "cinder/FrameGraph.hpp"
#include "cinder/Log.h"

namespace cinder {
namespace frame_graph {

typedef ref< class GLVideoINode > GLVideoINodeRef;
typedef ref< class GLVideoHapQDecodeShaderIONode > GLVideoHapQDecodeShaderIONodeRef;

class GLVideoINode : public TextureINode
{
public:

	static GLVideoINodeRef create( const glvideo::Context::ref & context, const ci::fs::path & path, bool playImmediately = true )
	{
		return std::make_shared< GLVideoINode >( context, path, playImmediately );
	}

    static GLVideoINodeRef create( const glvideo::Movie::ref & movie )
    {
        return std::make_shared< GLVideoINode >( movie );
    }

	GLVideoINode( const glvideo::Context::ref & context, const ci::fs::path & path, bool playImmediately = true );
	GLVideoINode( const glvideo::Movie::ref & movie );

	virtual void update() override;

	GLVideoINode & play() { mMovie->play(); return *this; }
	GLVideoINode & loop( bool enabled = true ) { mMovie->loop( enabled ); return *this; }
	GLVideoINode & stop() { mMovie->stop(); return *this; }
	GLVideoINode & seekToStart() { mMovie->seekToStart(); return *this; }
	GLVideoINode & seek( glvideo::seconds secs ) { mMovie->seek( secs ); return *this; }

    glvideo::seconds getElapsedTime() const { return mMovie->getElapsedTime(); }
	glvideo::seconds getRemainingTime() const { return mMovie->getRemainingTime(); }

    bool isPlaying() const { return mMovie->isPlaying(); }

	ci::vec2 getSize() { return ci::vec2( mMovie->getWidth(), mMovie->getHeight() ); }

    glvideo::Movie::ref getMovie() const { return mMovie; }

private:
	glvideo::Movie::ref mMovie;
};

class GLVideoHapQDecodeShaderIONode : public TextureShaderIONode<>
{
public:
    static GLVideoHapQDecodeShaderIONodeRef create( const ci::ivec2 & size )
    {
        return std::make_shared< GLVideoHapQDecodeShaderIONode >( size );
    }


    GLVideoHapQDecodeShaderIONode( const ci::ivec2 & size );
};


}
}