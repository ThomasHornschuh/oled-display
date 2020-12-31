#include "platform_conf.h"
#include "lauxlib.h"
#include "lrotable.h"
#include <string.h>
#include "encoding.h"
#include "lcd.h"
#include <stdbool.h>



// Retrieves a byte value (0..255) from the Lua stack
static uint8_t _lua_checkbyte( lua_State* L,int idx )
{
     int v = luaL_checkinteger( L, idx );
     luaL_argcheck( L, (v>=0 && v<=255) ,idx,  "allowed range 0..255" );
     return (uint8_t)v;
}

static int mod_oled_init( lua_State* L )
{
    int disp_on=luaL_optint(L,1,1);
    lcd_init(disp_on?LCD_DISP_ON:LCD_DISP_OFF); 
    lua_pushboolean( L, true );
    return 1;
}

static int mod_oled_clrscr( lua_State* L )
{
    lcd_clrscr();
    return 0;
}

static int mod_oled_gotoxy( lua_State* L )
{
    lcd_gotoxy(_lua_checkbyte( L, 1), _lua_checkbyte( L, 2) );
    return 0;
}

static int mod_oled_writetext(  lua_State* L )
{
    const char * str = luaL_checkstring(L,1);
    lcd_puts(str);   
    return 0;
}

#ifdef GRAPHICMODE
static int mod_oled_display(  lua_State* L  )
{
     lcd_display();
     return 0;
}

static int mod_oled_rect(  lua_State* L  )
{
   uint8_t px1 = _lua_checkbyte( L, 1 );
   uint8_t py1 = _lua_checkbyte( L, 2 );
   uint8_t px2 = _lua_checkbyte( L, 3 );
   uint8_t py2 = _lua_checkbyte( L, 4 );
   uint8_t color = _lua_checkbyte( L, 5 );
   int     fill = luaL_optint( L, 6, 0 );

   if (fill)
       lcd_fillRect(px1,py1,px2,py2,color);
    else
       lcd_drawRect(px1,py1,px2,py2,color); 
        
    return 0;

}

#endif

// Module function map
#define MIN_OPT_LEVEL 2
#include "lrodefs.h"
const LUA_REG_TYPE oled_map[] =
{
  { LSTRKEY( "init" ), LFUNCVAL( mod_oled_init ) },
  { LSTRKEY( "clrscr" ), LFUNCVAL( mod_oled_clrscr ) },
  { LSTRKEY( "gotoxy" ), LFUNCVAL( mod_oled_gotoxy ) },
  { LSTRKEY( "writetext" ), LFUNCVAL( mod_oled_writetext ) },
  #ifdef GRAPHICMODE
   { LSTRKEY( "display" ), LFUNCVAL( mod_oled_display ) },
   { LSTRKEY( "rect" ), LFUNCVAL( mod_oled_rect ) },
  #endif
  { LSTRKEY( "FILL" ), LNUMVAL( 1 ) },
  { LSTRKEY( "NO_FILL" ), LNUMVAL( 0 ) },
  //{ LSTRKEY( "debugger" ), LFUNCVAL( debugger ) },


  { LNILKEY, LNILVAL }
};