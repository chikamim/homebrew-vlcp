/*****************************************************************************
 * aribsub.c : ARIB subtitle subpicture decoder
 *****************************************************************************
 * Copyright (C) 2012 Naohiro KORIYAMA
 * Copyright (C) 2012 the VideoLAN team
 *
 * Authors:  Naohiro KORIYAMA <nkoriyama@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/
typedef struct arib_text_region_s
{
    char                      *psz_text;
    char                      *psz_html;

    char                      *psz_fontname;
    int                       i_font_color;
    int                       i_planewidth;
    int                       i_planeheight;
    int                       i_fontwidth;
    int                       i_fontheight;
    int                       i_verint;
    int                       i_horint;
    int                       i_charleft;
    int                       i_charbottom;

    struct arib_text_region_s *p_next;
} arib_text_region_t;

#define subpicture_updater_sys_t subpicture_sys_t

struct subpicture_updater_sys_t
{
    arib_text_region_t *p_region;
};

#if 0

static int SubpictureTextValidate(subpicture_t *subpic,
                                  bool has_src_changed, const video_format_t *fmt_src,
                                  bool has_dst_changed, const video_format_t *fmt_dst,
                                  mtime_t ts)
{
    subpicture_updater_sys_t *sys = subpic->updater.p_sys;
    VLC_UNUSED(fmt_src); VLC_UNUSED(fmt_dst); VLC_UNUSED(ts);
    VLC_UNUSED(sys);

    if (!has_src_changed && !has_dst_changed)
    {
        return VLC_SUCCESS;
    }
    return VLC_EGENERIC;
}
static void SubpictureTextUpdate(subpicture_t *subpic,
                                 const video_format_t *fmt_src,
                                 const video_format_t *fmt_dst,
                                 mtime_t ts)
{
    subpicture_updater_sys_t *sys = subpic->updater.p_sys;
    VLC_UNUSED(fmt_src); VLC_UNUSED(ts);

    if (fmt_dst->i_sar_num <= 0 || fmt_dst->i_sar_den <= 0)
    {
        return;
    }

    video_format_t fmt;
    video_format_Init(&fmt, VLC_CODEC_TEXT);
    fmt.i_sar_num = 1;
    fmt.i_sar_den = 1;

    subpicture_region_t *r = NULL;
    arib_text_region_t *p_region;
    for( p_region = sys->p_region; p_region; p_region = p_region->p_next )
    {
        if( !r )
        {
            subpic->p_region = r = subpicture_region_New(&fmt);
        }
        else
        {
            r->p_next = subpicture_region_New(&fmt);
            r = r->p_next;
        }
        if( r == NULL )
        {
            return;
        }

        r->psz_text = p_region->psz_text ? strdup(p_region->psz_text) : NULL;
        r->psz_html = p_region->psz_html ? strdup(p_region->psz_html) : NULL;
        r->i_align  = SUBPICTURE_ALIGN_LEFT | SUBPICTURE_ALIGN_TOP;

        subpic->i_original_picture_width  = p_region->i_planewidth;
        subpic->i_original_picture_height  = p_region->i_planeheight;

        r->i_x = p_region->i_charleft - (p_region->i_fontwidth + p_region->i_horint);
        r->i_y = p_region->i_charbottom - (p_region->i_fontheight + p_region->i_verint);
        r->p_style = text_style_New();
        r->p_style->psz_fontname = p_region->psz_fontname ? strdup( p_region->psz_fontname ) : NULL;
        r->p_style->i_font_size = p_region->i_fontheight;
        r->p_style->i_font_color = p_region->i_font_color;
//        r->p_style->i_spacing = p_region->i_horint + p_region->i_fontwidth;
        //r->p_style->i_style_flags = STYLE_BOLD | STYLE_ITALIC | STYLE_UNDERLINE | STYLE_STRIKEOUT;
        r->p_style->i_style_flags = STYLE_BOLD;
        r->p_style->i_spacing = p_region->i_horint;
        r->p_style->i_font_width = p_region->i_fontwidth;
        r->p_style->i_font_height = p_region->i_fontheight;
    }
}
#endif

static void SubpictureTextDestroy(subpicture_t *subpic)
{
    subpicture_updater_sys_t *sys = subpic->p_sys;

    arib_text_region_t *p_region, *p_region_next;
    for( p_region = sys->p_region; p_region; p_region = p_region_next )
    {
        free( p_region->psz_text );
        free( p_region->psz_html );
        free( p_region->psz_fontname );
        p_region_next = p_region->p_next;
        free( p_region );
    }
    sys->p_region = NULL;
    free( sys );
}

#if 0
static inline subpicture_t *decoder_NewSubpictureText(decoder_t *decoder)
{
    subpicture_updater_sys_t *sys = (subpicture_updater_sys_t*)
        calloc( 1, sizeof(subpicture_updater_sys_t) );
    subpicture_updater_t updater = {
        .pf_validate = SubpictureTextValidate,
        .pf_update   = SubpictureTextUpdate,
        .pf_destroy  = SubpictureTextDestroy,
        .p_sys       = sys,
    };
    subpicture_t *subpic = decoder_NewSubpicture(decoder, &updater);
    if( subpic == NULL )
    {
        free( sys );
    }
    return subpic;
}
#endif
