# http://www.easyrgb.com/index.php?X=MATH&H=18#text18
# http://www.w3.org/TR/2011/REC-css3-color-20110607/#ABC

def _check(value, valuetype, minvalue, maxvalue, valuename):
    typeexception = "'{0}' should be of type '{1}'".format( valuename
                                                          , valuetype.__name__
                                                          )
    rangeexception = "{0} <= '{1}' <= {2}".format( minvalue
                                                 , valuename
                                                 , maxvalue
                                                 )
    if not isinstance(value, valuetype)  : raise TypeError(typeexception)
    if minvalue <= value <= maxvalue: return
    raise ValueError(rangeexception)

class Color(int):
    def __init__(self, red, green, blue, alpha):
        _check(hue, float, 0.0, 1.0, "hue")
        _check(saturation, float, 0.0, 1.0, "saturation")
        _check(lightness, float, 0.0, 1.0, "lightness")
        _check(alpha, float, 0.0, 1.0, "alpha")
        self.hue   = hue
        self.saturation = saturation
        self.lightness  = lightness
        self.alpha = alpha

    # def lighter():
    #     pass

    # def darker():
    #     pass

    # def __str__():
    #     pass

    # def show():
    #     pass

    def hsl(self):
        return self.hsla()[0:3]

    def hsla(self):
        return ( self.hue        * 360.0
               , self.saturation * 100.0
               , self.lightness  * 100.0
               , self.alpha
               )

    def rgb(self):
        return self.rgba()[0:3]

    def rgba(self):
        def hue_to_rgb(m1, m2, hue):
            hue = hue + 1.0 if hue < 0.0 else hue - 1.0
            if hue * 6.0 < 1.0 : return m1 + (m2 - m1) * hue * 6.0
            if hue * 2.0 < 1.0 : return m2
            if hue * 3.0 < 2.0 : return m1 + (m2 - m1) * (2.0/3.0 - hue) * 6.0
            return m1

        lightness = lightness / 100.0
        saturation = saturation / 100.0

        if lightness <= 0.5: m2 = lightness * (1.0 + saturation)
        else               : m2 = lightness * (1.0 - saturation) + saturation
        m1 = lightness * 2.0 - m2
        return ( hue_to_rgb(m1, m2, hue + 1.0/3.0) * 255
               , hue_to_rgb(m1, m2, hue          ) * 255
               , hue_to_rgb(m1, m2, hue - 1.0/3.0) * 255
               , self.alpha
               )

    def hex(self):
        return self.hex8()

    def hex8(self):
        (r,g,b,a) = self.rgba()
        a = int(a * 255)
        return ("#" + hex(r)[2:] + hex(g)[2:] + hex(b)[2:] + hex(a)[2:]).upper()

    def hex3(self):
        (r,g,b,a) = self.rgba()
        a = int(a * 255)
        return ("#" + hex(r)[2:] + hex(g)[2:] + hex(b)[2:] + hex(a)[2:]).upper()

    def hex4(self):
        (r,g,b,a) = self.rgba()
        a = int(a * 255)
        return ("#" + hex(r)[2:] + hex(g)[2:] + hex(b)[2:] + hex(a)[2:]).upper()

    def hex6(self):
        (r,g,b,a) = self.rgba()
        a = int(a * 255)
        return ("#" + hex(r)[2:] + hex(g)[2:] + hex(b)[2:] + hex(a)[2:]).upper()



    # def cmyk():pass


def RGB(red, green, blue):
    return RGBA(red, green, blue, 1.0)

def RGBA(red, green, blue, alpha):
    _check(red, int, 0, 255, "red")
    _check(green, int, 0, 255, "green")
    _check(blue, int, 0, 255, "blue")
    _check(alpha, float, 0.0, 1.0, "alpha")

    red     = red   / 255.0
    green   = green / 255.0
    blue    = blue  / 255.0
    minval  = min(red, green, blue)
    maxval  = max(red, green, blue)
    delta   = maxval - minval

    lightnes = ( maxval + minval ) / 2.0

    if delta == 0.0:
        hue = 0.0
        saturation = 0.0
    else:
       if lightness < 0.5 : saturation = delta / (maxval + minval)
       else               : saturation = delta / ( 2.0 - maxval - minval )

       deltar = (((maxval - red) / 6.0 ) + (delta /2.0)) / delta
       deltag = (((maxval - green) / 6.0 ) + (delta / 2.0) ) / delta
       deltab = (((maxval - blue) / 6.0 ) + (delta / 2.0) ) / delta

       if   red   == maxval : hue =           deltab - deltag
       elif green == maxval : hue = 1.0/3.0 + deltar - deltab
       elif blue  == maxval : hue = 2.0/3.0 + deltag - deltar

       if hue < 0.0 : hue += 1.0
       if hue > 1.0 : hue -= 1.0

    return Color(hue, saturation, lightness, alpha)

def HEX(hexstring):
    if not hexstring.startswith("#") :
        raise ValueError("'{0}' should start with '#'".format(hexstring))
    if len(hexstring) == 4:
        return RGB( int(hexstring[1], 16) * 17
                  , int(hexstring[2], 16) * 17
                  , int(hexstring[3], 16) * 17
                  )
    elif len(hexstring) == 5:
        return RGBA( int(hexstring[1], 16) * 17
                   , int(hexstring[2], 16) * 17
                   , int(hexstring[3], 16) * 17
                   , int(hexstring[4], 16) / 15.0
                   )
    elif len(hexstring) == 7:
        return RGB( int(hexstring[1:3], 16)
                  , int(hexstring[3:5], 16)
                  , int(hexstring[5:7], 16)
                  )
    elif len(hexstring) == 9:
        return RGBA( int(hexstring[1:3], 16)
                   , int(hexstring[3:5], 16)
                   , int(hexstring[5:7], 16)
                   , int(hexstring[7:9], 16) / 255.0
                   )
    else:
        raise ValueError("Valid formats are #RGB, #RGBA, #RRGGBB, #RRGGBBAA")

def HSLA(hue, saturation, lightness, alpha):
    _check(hue, float, 0.0, 360.0, "hue")
    _check(saturation, float, 0.0, 100.0, "saturation")
    _check(lightness, float, 0.0, 100.0, "lightness")
    _check(alpha, float, 0.0, 1.0, "alpha")

    return Color( hue/360.0
                , saturation/100.0
                , lightness/100.0
                , alpha
                )

def HSL(hue, saturation, lightness):
    return HSLA(hue, saturation, lightness, 1.0)

# def CMYK(cyan, magenta, yellow, key):
#     return RGBA( 255 - cyan
#                , 255 - magenta
#                , 255 - yellow

#                )

# def CMY(cyan, magenta, yellow):
#     return CMYK(cyan, magenta, yellow, 1.0)


# def HSV(hue, saturation, value):
#     pass


# PINK COLORS
PINK                    = RGB(255, 192, 203)
LIGHT_PINK              = RGB(255, 182, 193)
HOT_PINK                = RGB(255, 105, 180)
DEEP_PINK               = RGB(255, 20,  147)
PALE_VIOLET_RED         = RGB(219, 112, 147)
MEDIUM_VIOLET_RED       = RGB(199, 21,  133)

# RED COLOURS
LIGHT_SALMON            = RGB(255, 160, 122)
SALMON                  = RGB(250, 128, 114)
DARK_SALMON             = RGB(233, 150, 122)
LIGHT_CORAL             = RGB(240, 128, 128)
INDIAN_RED              = RGB(205,  92,  92)
CRIMSON                 = RGB(220,  20,  60)
FIREBRICK               = RGB(178,  34,  34)
DARK_RED                = RGB(139,   0,   0)
RED                     = RGB(255,   0,   0)

#ORANGE COLOURS
ORANGE_RED              = RGB(255,  69,   0)
TOMATO                  = RGB(255,  99,  71)
CORAL                   = RGB(255, 127,  80)
DARK_ORANGE             = RGB(255, 140,   0)
ORANGE                  = RGB(255, 165,   0)

#YELLOW COLOURS
YELLOW                  = RGB(255, 255,   0)
LIGHT_YELLOW            = RGB(255, 255, 224)
LEMON_CHIFFON           = RGB(255, 250, 205)
LIGHT_GOLDENROD_YELLOW  = RGB(250, 250, 210)
PAPAYA_WHIP             = RGB(255, 239, 213)
MOCCASIN                = RGB(255, 228, 181)
PEACH_PUFF              = RGB(255, 218, 185)
PALE_GOLDENROD          = RGB(238, 232, 170)
KHAKI                   = RGB(240, 230, 140)
DARK_KHAKI              = RGB(189, 183, 107)
GOLD                    = RGB(255, 215,   0)

# BROWN COLOURS
CORN_SILK               = RGB(255, 248, 220)
BLANCHED_ALMOND         = RGB(255, 235, 205)
BISQUE                  = RGB(255, 228, 196)
NAVAJO_WHITE            = RGB(255, 222, 173)
WHEAT                   = RGB(245, 222, 179)
BURLY_WOOD              = RGB(222, 184, 135)
TAN                     = RGB(210, 180, 140)
ROSY_BROWN              = RGB(188, 143, 143)
SANDY__BROWN            = RGB(244, 164,  96)
GOLDENROD               = RGB(218, 165,  32)
DARK_GOLDENROD          = RGB(184, 134,  11)
PERU                    = RGB(205, 133,  63)
CHOCOLATE               = RGB(210, 105,  30)
SADDLE_BROWN            = RGB(139,  69,  19)
SIENNA                  = RGB(160,  82,  45)
BROWN                   = RGB(165,  42,  42)
MAROON                  = RGB(128,   0,   0)

# GREEN COLOURS
DARK_OLIVE_GREEN        = RGB( 85, 107,  47)
OLIVE                   = RGB(128, 128,   0)
OLIVE_DRAB              = RGB(107, 142,  35)
YELLOW_GREEN            = RGB(154, 205,  50)
LIME_GREEN              = RGB( 50, 205,  50)
LIME                    = RGB(  0, 255,   0)
LAWN_GREEN              = RGB(124, 252,   0)
CHARTREUSE              = RGB(127, 255,   0)
GREEN_YELLOW            = RGB(173, 255,  47)
SPRING_GREEN            = RGB(  0, 255, 127)
MEDIUM_SPRING_GREEN     = RGB(  0, 250, 154)
LIGHT_GREEN             = RGB(144, 238, 144)
PALE_GREEN              = RGB(152, 251, 152)
DARK_SEA_GREEN          = RGB(143, 188, 143)
MEDIUM_SEA_GREEN        = RGB( 60, 179, 113)
SEA_GREEN               = RGB( 46, 139,  87)
FOREST_GREEN            = RGB( 34, 139,  34)
GREEN                   = RGB(  0, 128,   0)
DARK_GREEN              = RGB(  0, 100,   0)

# CYAN COLOURS
MEDIUM_AQUAMARINE       = RGB(102, 205, 170)
AQUA                    = RGB(  0, 255, 255)
CYAN                    = RGB(  0, 255, 255)
LIGHT_CYAN              = RGB(224, 255, 255)
PALE_TURQUOISE          = RGB(175, 238, 238)
AQUA_MARINE             = RGB(127, 255, 212)
TURQUOISE               = RGB( 64, 224, 208)
MEDIUM_TURQUOISE        = RGB( 72, 209, 204)
DARK_TURQUOISE          = RGB(  0, 206, 209)
LIGHT_SEA_GREEN         = RGB( 32, 178, 170)
CADET_BLUE              = RGB( 95, 158, 160)
DARK_CYAN               = RGB(  0, 139, 139)
TEAL                    = RGB(  0, 128, 128)

# BLUE COLOURS
LIGHT_STEEL_BLUE        = RGB(176, 196, 222)
POWDER_BLUE             = RGB(176, 224, 230)
LIGHT_BLUE              = RGB(173, 216, 230)
SKY_BLUE                = RGB(135, 206, 235)
LIGHT_SKY_BLUE          = RGB(135, 206, 250)
DEEP_SKY_BLUE           = RGB(  0, 191, 255)
DODGER_BLUE             = RGB( 30, 144, 255)
CORNFLOWER_BLUE         = RGB(100, 149, 237)
STEEL_BLUE              = RGB( 70, 130, 180)
ROYAL_BLUE              = RGB( 65, 105, 225)
BLUE                    = RGB(  0,   0, 255)
MEDIUM_BLUE             = RGB(  0,   0, 205)
DARK_BLUE               = RGB(  0,   0, 139)
NAVY                    = RGB(  0,   0, 128)
MIDNIGHT_BLUE           = RGB( 25,  25, 112)

# PURPLE/VIOLET/MAGENTA COLOURS
LAVENDER                = RGB(230, 230, 250)
THISTLE                 = RGB(216, 191, 216)
PLUM                    = RGB(221, 160, 221)
VIOLET                  = RGB(238, 130, 238)
ORCHID                  = RGB(218, 112, 214)
FUCHSIA                 = RGB(255,   0, 255)
MAGENTA                 = RGB(255,   0, 255)
MEDIUM_ORCHID           = RGB(186,  85, 211)
MEDIUM_PURPLE           = RGB(147, 112, 219)
BLUE_VIOLET             = RGB(138,  43, 226)
DARK_VIOLET             = RGB(148,   0, 211)
DARK_ORCHID             = RGB(153,  50, 204)
DARK_MAGENTA            = RGB(139,   0, 139)
PURPLE                  = RGB(128,   0, 128)
INDIGO                  = RGB( 75,   0, 130)
DARK_SLATE_BLUE         = RGB( 72,  61, 139)
REBECCA_PURPLE          = RGB(102,  51, 153)
SLATE_BLUE              = RGB(106,  90, 205)
MEDIUM_SLATE_BLUE       = RGB(123, 104, 238)

#WHITE COLOURS
WHITE                   = RGB(255, 255, 255)
SNOW                    = RGB(255, 250, 250)
HONEYDEW                = RGB(240, 255, 240)
MINT_CREAM              = RGB(245, 255, 250)
AZURE                   = RGB(240, 255, 255)
ALICE_BLUE              = RGB(240, 248, 255)
GHOST_WHITE             = RGB(248, 248, 255)
WHITE_SMOKE             = RGB(245, 245, 245)
SEA_SHELL               = RGB(255, 245, 238)
BEIGE                   = RGB(245, 245, 220)
OLD_LACE                = RGB(253, 245, 230)
FLORAL_WHITE            = RGB(255, 250, 240)
IVORY                   = RGB(255, 255, 240)
ANTIQUE_WHITE           = RGB(250, 235, 215)
LINEN                   = RGB(250, 240, 230)
LAVENDER_BLUSH          = RGB(255, 240, 245)
MISTY_ROSE              = RGB(255, 228, 225)

# GRAY/BLACK COLOURS
GAINSBORO               = RGB(220, 220, 220)
LIGHT_GREY              = RGB(211, 211, 211)
SILVER                  = RGB(192, 192, 192)
DARK_GRAY               = RGB(169, 169, 169)
GRAY                    = RGB(128, 128, 128)
DIM_GRAY                = RGB(105, 105, 105)
LIGHT_SLATE_GRAY        = RGB(119, 136, 153)
SLATE_GRAY              = RGB(112, 128, 144)
DARK_SLATE_GRAY         = RGB( 47,  79,  79)
BLACK                   = RGB(  0,   0,   0)
