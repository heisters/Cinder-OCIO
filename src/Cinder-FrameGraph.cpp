#include "Cinder-FrameGraph.h"
#include <algorithm>
#include "cinder/Log.h"
#include "cinder/GeomIo.h"
#include "cinder/gl/gl.h"

using namespace cinder;
using namespace frame_graph;
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// SurfaceINode

SurfaceINode::SurfaceINode( const Surface32fRef & surface ) :
mSurface( surface )
{
}

//void SurfaceINode::update()
//{
//	for ( auto & output : mOutputs ) output->update( mSurface );
//}


////////////////////////////////////////////////////////////////////////////////
// TextureINode

//void TextureINode::update()
//{
//	for ( auto & output : mOutputs ) output->update( mOutTex );
//}

//void TextureINode::update( const gl::Texture2dRef & texture )
//{
//	mOutTex = texture;
//	for ( auto & output : mOutputs ) output->update( mOutTex );
//}

////////////////////////////////////////////////////////////////////////////////
// TextureONode

TextureONode::TextureONode()
{}

//void TextureONode::update( const Surface32fRef & image )
//{
//	mTexture = gl::Texture2d::create( *image );
//}
//
//void TextureONode::update( const gl::Texture2dRef & texture )
//{
//	mTexture = texture;
//}
////////////////////////////////////////////////////////////////////////////////
// TextureIONode

void TextureIONode::update( const gl::Texture2dRef & texture )
{
    TextureONode::update( texture );
    TextureINode::update( texture );
}

////////////////////////////////////////////////////////////////////////////////
// TextureShaderIONode

TextureShaderIONode::TextureShaderIONode( const gl::GlslProgRef & shader )
{
	mBatch = gl::Batch::create( geom::Rect() >> geom::Translate( 0.5f, 0.5f ), shader );
}

void TextureShaderIONode::update( const ci::gl::Texture2dRef & texture )
{
	ivec2 size = texture->getSize();


	if ( ! mFbo || mFbo->getSize() != size ) {
		mFbo = gl::Fbo::create( size.x, size.y );
		mModelMatrix = scale( vec3( mFbo->getSize(), 1.f ) );
	}

	{
		gl::ScopedFramebuffer	scp_fbo( mFbo );
		gl::ScopedViewport		scp_viewport( mFbo->getSize() );
		gl::ScopedMatrices		scp_mtx;
		gl::ScopedTextureBind	scp_tex( texture, 0 );
        gl::ScopedColor         scp_color( ColorAf( 1.f, 1.f, 1.f, 1.f ) );

		gl::setMatricesWindow( mFbo->getSize() );
		gl::multModelMatrix( mModelMatrix );

		mBatch->getGlslProg()->uniform( "uSize", vec2( mFbo->getSize() ) );

		gl::clear();

		mBatch->draw();
	}


	auto tex = mFbo->getColorTexture();
	tex->setTopDown( true );
	TextureIONode::update( tex );
}
