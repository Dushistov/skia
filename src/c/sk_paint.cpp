/*
 * Copyright 2015 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "SkBlendMode.h"
#include "SkMaskFilter.h"
#include "SkPaint.h"
#include "SkShader.h"
#include "SkDashPathEffect.h"

#include "sk_paint.h"
#include "sk_types_priv.h"

#define MAKE_FROM_TO_NAME(FROM)     g_ ## FROM ## _map

const struct {
    sk_stroke_cap_t fC;
    SkPaint::Cap    fSK;
} MAKE_FROM_TO_NAME(sk_stroke_cap_t)[] = {
    { BUTT_SK_STROKE_CAP,   SkPaint::kButt_Cap   },
    { ROUND_SK_STROKE_CAP,  SkPaint::kRound_Cap  },
    { SQUARE_SK_STROKE_CAP, SkPaint::kSquare_Cap },
};

const struct {
    sk_stroke_join_t fC;
    SkPaint::Join    fSK;
} MAKE_FROM_TO_NAME(sk_stroke_join_t)[] = {
    { MITER_SK_STROKE_JOIN, SkPaint::kMiter_Join },
    { ROUND_SK_STROKE_JOIN, SkPaint::kRound_Join },
    { BEVEL_SK_STROKE_JOIN, SkPaint::kBevel_Join },
};

#define CType           sk_stroke_cap_t
#define SKType          SkPaint::Cap
#define CTypeSkTypeMap  MAKE_FROM_TO_NAME(sk_stroke_cap_t)
#include "sk_c_from_to.h"

#define CType           sk_stroke_join_t
#define SKType          SkPaint::Join
#define CTypeSkTypeMap  MAKE_FROM_TO_NAME(sk_stroke_join_t)
#include "sk_c_from_to.h"

//////////////////////////////////////////////////////////////////////////////////////////////////

sk_paint_t* sk_paint_new() { return (sk_paint_t*)new SkPaint; }

void sk_paint_delete(sk_paint_t* cpaint) { delete AsPaint(cpaint); }

bool sk_paint_is_antialias(const sk_paint_t* cpaint) {
    return AsPaint(*cpaint).isAntiAlias();
}

void sk_paint_set_antialias(sk_paint_t* cpaint, bool aa) {
    AsPaint(cpaint)->setAntiAlias(aa);
}

sk_color_t sk_paint_get_color(const sk_paint_t* cpaint) {
    return AsPaint(*cpaint).getColor();
}

void sk_paint_set_color(sk_paint_t* cpaint, sk_color_t c) {
    AsPaint(cpaint)->setColor(c);
}

void sk_paint_set_shader(sk_paint_t* cpaint, sk_shader_t* cshader) {
    AsPaint(cpaint)->setShader(sk_ref_sp(AsShader(cshader)));
}

void sk_paint_set_maskfilter(sk_paint_t* cpaint, sk_maskfilter_t* cfilter) {
    AsPaint(cpaint)->setMaskFilter(sk_ref_sp(AsMaskFilter(cfilter)));
}

bool sk_paint_is_stroke(const sk_paint_t* cpaint) {
    return AsPaint(*cpaint).getStyle() != SkPaint::kFill_Style;
}

void sk_paint_set_stroke(sk_paint_t* cpaint, bool doStroke) {
    AsPaint(cpaint)->setStyle(doStroke ? SkPaint::kStroke_Style : SkPaint::kFill_Style);
}

float sk_paint_get_stroke_width(const sk_paint_t* cpaint) {
    return AsPaint(*cpaint).getStrokeWidth();
}

void sk_paint_set_stroke_width(sk_paint_t* cpaint, float width) {
    AsPaint(cpaint)->setStrokeWidth(width);
}

float sk_paint_get_stroke_miter(const sk_paint_t* cpaint) {
    return AsPaint(*cpaint).getStrokeMiter();
}

void sk_paint_set_stroke_miter(sk_paint_t* cpaint, float miter) {
    AsPaint(cpaint)->setStrokeMiter(miter);
}

sk_stroke_cap_t sk_paint_get_stroke_cap(const sk_paint_t* cpaint) {
    sk_stroke_cap_t ccap;
    if (find_c(AsPaint(*cpaint).getStrokeCap(), &ccap)) {
        ccap = BUTT_SK_STROKE_CAP;
    }
    return ccap;
}

void sk_paint_set_stroke_cap(sk_paint_t* cpaint, sk_stroke_cap_t ccap) {
    SkPaint::Cap skcap;
    if (find_sk(ccap, &skcap)) {
        AsPaint(cpaint)->setStrokeCap(skcap);
    } else {
        // unknown ccap
    }
}

sk_stroke_join_t sk_paint_get_stroke_join(const sk_paint_t* cpaint) {
    sk_stroke_join_t cjoin;
    if (find_c(AsPaint(*cpaint).getStrokeJoin(), &cjoin)) {
        cjoin = MITER_SK_STROKE_JOIN;
    }
    return cjoin;
}

void sk_paint_set_stroke_join(sk_paint_t* cpaint, sk_stroke_join_t cjoin) {
    SkPaint::Join skjoin;
    if (find_sk(cjoin, &skjoin)) {
        AsPaint(cpaint)->setStrokeJoin(skjoin);
    } else {
        // unknown cjoin
    }
}

void sk_paint_set_xfermode_mode(sk_paint_t* paint, sk_xfermode_mode_t mode) {
    SkASSERT(paint);
    SkBlendMode skmode;
    switch (mode) {
        #define MAP(X, Y) case (X): skmode = (Y); break
        MAP( CLEAR_SK_XFERMODE_MODE,      SkBlendMode::kClear      );
        MAP( SRC_SK_XFERMODE_MODE,        SkBlendMode::kSrc        );
        MAP( DST_SK_XFERMODE_MODE,        SkBlendMode::kDst        );
        MAP( SRCOVER_SK_XFERMODE_MODE,    SkBlendMode::kSrcOver    );
        MAP( DSTOVER_SK_XFERMODE_MODE,    SkBlendMode::kDstOver    );
        MAP( SRCIN_SK_XFERMODE_MODE,      SkBlendMode::kSrcIn      );
        MAP( DSTIN_SK_XFERMODE_MODE,      SkBlendMode::kDstIn      );
        MAP( SRCOUT_SK_XFERMODE_MODE,     SkBlendMode::kSrcOut     );
        MAP( DSTOUT_SK_XFERMODE_MODE,     SkBlendMode::kDstOut     );
        MAP( SRCATOP_SK_XFERMODE_MODE,    SkBlendMode::kSrcATop    );
        MAP( DSTATOP_SK_XFERMODE_MODE,    SkBlendMode::kDstATop    );
        MAP( XOR_SK_XFERMODE_MODE,        SkBlendMode::kXor        );
        MAP( PLUS_SK_XFERMODE_MODE,       SkBlendMode::kPlus       );
        MAP( MODULATE_SK_XFERMODE_MODE,   SkBlendMode::kModulate   );
        MAP( SCREEN_SK_XFERMODE_MODE,     SkBlendMode::kScreen     );
        MAP( OVERLAY_SK_XFERMODE_MODE,    SkBlendMode::kOverlay    );
        MAP( DARKEN_SK_XFERMODE_MODE,     SkBlendMode::kDarken     );
        MAP( LIGHTEN_SK_XFERMODE_MODE,    SkBlendMode::kLighten    );
        MAP( COLORDODGE_SK_XFERMODE_MODE, SkBlendMode::kColorDodge );
        MAP( COLORBURN_SK_XFERMODE_MODE,  SkBlendMode::kColorBurn  );
        MAP( HARDLIGHT_SK_XFERMODE_MODE,  SkBlendMode::kHardLight  );
        MAP( SOFTLIGHT_SK_XFERMODE_MODE,  SkBlendMode::kSoftLight  );
        MAP( DIFFERENCE_SK_XFERMODE_MODE, SkBlendMode::kDifference );
        MAP( EXCLUSION_SK_XFERMODE_MODE,  SkBlendMode::kExclusion  );
        MAP( MULTIPLY_SK_XFERMODE_MODE,   SkBlendMode::kMultiply   );
        MAP( HUE_SK_XFERMODE_MODE,        SkBlendMode::kHue        );
        MAP( SATURATION_SK_XFERMODE_MODE, SkBlendMode::kSaturation );
        MAP( COLOR_SK_XFERMODE_MODE,      SkBlendMode::kColor      );
        MAP( LUMINOSITY_SK_XFERMODE_MODE, SkBlendMode::kLuminosity );
        #undef MAP
        default:
            return;
    }
    AsPaint(paint)->setBlendMode(skmode);
}

void sk_paint_set_text_size(sk_paint_t *paint, float ts)
{
    AsPaint(paint)->setTextSize(ts);
}

void sk_paint_measure_text(const sk_paint_t *paint, const char *text, size_t text_len, sk_rect_t *bounds)
{
   SkRect rect = SkRect::MakeEmpty();
   AsPaint(paint)->measureText(text, text_len, &rect);
   if (bounds != nullptr) {
       bounds->left = rect.fLeft;
       bounds->right = rect.fRight;
       bounds->top = rect.fTop;
       bounds->bottom = rect.fBottom;
   }
}

float sk_paint_get_font_spacing(const sk_paint_t *paint)
{
    return AsPaint(paint)->getFontSpacing();
}

extern "C" void sk_paint_set_dash_effect(sk_paint_t *paint, float value1, float value2)
{
    const SkScalar intervals[] = { value1, value2 };
    size_t count  = sizeof(intervals) / sizeof(intervals[0]);
    AsPaint(paint)->setPathEffect(SkDashPathEffect::Make(intervals, count, 0.0f));
}

void sk_paint_reset_dash_effect(sk_paint_t *paint)
{
    AsPaint(paint)->setPathEffect(nullptr);
}
