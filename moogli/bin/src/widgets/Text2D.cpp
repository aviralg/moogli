class Text2D
{
private:
    Text();

    void
    set_color(const osg::Vec4 & color);

    const osg::Vec4 &
    get_color() const;

    void
    set_font(const std::string & fontfile);

    const std::string &
    get_font();

    void 	setStyle (Style *style)
 	Set the text style. 
Style * 	getStyle ()
 	Get the text style. 
const Style * 	getStyle () const
 	Get the const text style. 
Style * 	getOrCreateStyle ()
 	Get or create the text style. 
void 	setFontResolution (unsigned int width, unsigned int height)
 	Set the Font reference width and height resolution in texels. 
unsigned int 	getFontWidth () const
unsigned int 	getFontHeight () const
void 	setText (const String &text)
 	Set the text using a osgText::String. 
void 	setText (const std::string &text)
 	Set the text using a std::string, which is converted to an internal TextString. 
void 	setText (const std::string &text, String::Encoding encoding)
 	Set the text using a Unicode encoded std::string, which is converted to an internal TextString. 
void 	setText (const wchar_t *text)
 	Set the text using a wchar_t string, which is converted to an internal TextString. 
String & 	getText ()
 	Get the text string. 
const String & 	getText () const
 	Get the const text string. 
void 	update ()
 	update internal glyph respresentation used for rendering, and bounding volume. 
void 	setCharacterSize (float height)
 	Set the rendered character size in object coordinates. 
void 	setCharacterSize (float height, float aspectRatio)
 	Set the rendered character size in object coordinates. 
float 	getCharacterHeight () const
float 	getCharacterAspectRatio () const
void 	setCharacterSizeMode (CharacterSizeMode mode)
 	Set how the CharacterSize value relates to the final rendered character. 
CharacterSizeMode 	getCharacterSizeMode () const
 	Get the CharacterSizeMode. 
void 	setMaximumWidth (float maximumWidth)
 	Set the maximum width of the text box. 
float 	getMaximumWidth () const
 	Get the maximim width of the text box. 
void 	setMaximumHeight (float maximumHeight)
 	Set the maximum height of the text box. 
float 	getMaximumHeight () const
 	Get the maximum height of the text box. 
void 	setLineSpacing (float lineSpacing)
 	Set the line spacing of the text box, given as a percentage of the character height. 
float 	getLineSpacing () const
 	Get the line spacing of the text box. 
void 	setPosition (const osg::Vec3 &pos)
 	Set the position of text. 
const osg::Vec3 & 	getPosition () const
 	Get the position of text. 
void 	setAlignment (AlignmentType alignment)
AlignmentType 	getAlignment () const
void 	setAxisAlignment (AxisAlignment axis)
AxisAlignment 	getAxisAlignment () const
void 	setRotation (const osg::Quat &quat)
const osg::Quat & 	getRotation () const
void 	setAutoRotateToScreen (bool autoRotateToScreen)
bool 	getAutoRotateToScreen () const
void 	setLayout (Layout layout)
Layout 	getLayout () const
void 	setDrawMode (unsigned int mode)
unsigned int 	getDrawMode () const
void 	setBoundingBoxMargin (float margin)
float 	getBoundingBoxMargin () const
void 	setBoundingBoxColor (const osg::Vec4 &color)
const osg::Vec4 & 	getBoundingBoxColor () const
void 	setKerningType (KerningType kerningType)
KerningType 	getKerningType () const
unsigned int 	getLineCount () const
 	Get the number of wrapped lines - only valid after computeGlyphRepresentation() has been called, returns 0 otherwise. 
virtual void 	setThreadSafeRefUnref (bool threadSafe)
 	Set whether to use a mutex to ensure ref() and unref() are thread safe. 
virtual void 	resizeGLObjectBuffers (unsigned int maxSize)
 	Resize any per context GLObject buffers to specified size. 
virtual void 	releaseGLObjects (osg::State *state=0) const
 	If State is non-zero, this function releases OpenGL objects for the specified graphics context. 
virtual osg::BoundingBox 	computeBound () const



  Text( const Text & text
        , const osg::CopyOp &copyop=osg::CopyOp::SHALLOW_COPY)

    virtual void
    setFont (Font *font=0)
 	Set the Font to use to render the text. 
virtual void 	setFont (osg::ref_ptr< Font > font)
 	Set the Font to use to render the text. 
virtual void 	setFont (const std::string &fontfile)
 	Set the font, loaded from the specified front file, to use to render the text, setFont("") sets the use of the default font. 
void 	setEnableDepthWrites (bool enable)
 	Turns off writing to the depth buffer when rendering text. 
bool 	getEnableDepthWrites () const
void 	setBackdropType (BackdropType type)
 	BackdropType gives you a background shadow text behind your regular text. 
BackdropType 	getBackdropType () const
void 	setBackdropOffset (float offset=0.07f)
 	Sets the amount text is offset to create the backdrop/shadow effect. 
void 	setBackdropOffset (float horizontal, float vertical)
 	This overloaded version lets you specify the offset for the horizontal and vertical components separately. 
float 	getBackdropHorizontalOffset () const
float 	getBackdropVerticalOffset () const
void 	setBackdropColor (const osg::Vec4 &color)
 	This specifies the color of the backdrop text. 
const osg::Vec4 & 	getBackdropColor () const
void 	setBackdropImplementation (BackdropImplementation implementation)
 	This specifies the underlying backdrop rendering implementation. 
BackdropImplementation 	getBackdropImplementation () const
void 	setColorGradientMode (ColorGradientMode mode)
 	This sets different types of text coloring modes. 
ColorGradientMode 	getColorGradientMode () const
void 	setColorGradientCorners (const osg::Vec4 &topLeft, const osg::Vec4 &bottomLeft, const osg::Vec4 &bottomRight, const osg::Vec4 &topRight)
 	Used only for gradient mode, let's you specify the colors of the 4 corners. 
const osg::Vec4 & 	getColorGradientTopLeft () const
const osg::Vec4 & 	getColorGradientBottomLeft () const
const osg::Vec4 & 	getColorGradientBottomRight () const
const osg::Vec4 & 	getColorGradientTopRight () const
