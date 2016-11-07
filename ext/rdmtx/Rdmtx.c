/*
Rdmtx - Ruby wrapper for libdmtx

Copyright (C) 2008 Romain Goyet

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

Contact: r.goyet@gmail.com
*/

/* $Id:$ */

#include <ruby.h>
#include <dmtx.h>

static VALUE rdmtx_init(VALUE self) {
    return self;
}

static VALUE rdmtx_decode(VALUE self, VALUE image /* Image from RMagick (Magick::Image) */, VALUE timeout /* Timeout in msec */) {

    VALUE rawImageString = rb_funcall(image, rb_intern("export_pixels_to_str"), 0);

    VALUE safeImageString = StringValue(rawImageString);

    char * imageBuffer = RSTRING_PTR(safeImageString);

    int width = NUM2INT(rb_funcall(image, rb_intern("columns"), 0));
    int height = NUM2INT(rb_funcall(image, rb_intern("rows"), 0));

    DmtxImage *dmtxImage = dmtxImageCreate((unsigned char *)imageBuffer, width,
          height, DmtxPack24bppRGB);

    VALUE results = rb_ary_new();

    /* Initialize decode struct for newly loaded image */
    DmtxDecode * decode = dmtxDecodeCreate(dmtxImage, 1);

    DmtxRegion * region;
    DmtxMessage * message;

    int intTimeout = NUM2INT(timeout);
    DmtxTime dmtxTimeout = dmtxTimeAdd(dmtxTimeNow(), intTimeout);

    for(;;) {
        if (intTimeout == 0) {
            region = dmtxRegionFindNext(decode, NULL);
        } else {
            region = dmtxRegionFindNext(decode, &dmtxTimeout);
        }

        if (region == NULL )
            break;

        message = dmtxDecodeMatrixRegion(decode, region, DmtxUndefined);
        if (message != NULL) {
            VALUE outputString = rb_str_new2((char *)message->output);
            rb_ary_push(results, outputString);
            dmtxMessageDestroy(&message);
        }

        dmtxRegionDestroy(&region);
    }

    dmtxDecodeDestroy(&decode);
    dmtxImageDestroy(&dmtxImage);

    return results;
}

static VALUE rdmtx_encode(int argc, VALUE * argv, VALUE self) {

    VALUE string, margin, module, size;
    VALUE safeString;
    VALUE magickImageClass;
    VALUE outputImage;

    int safeMargin, safeModule, safeSize;
    int width;
    int height;
    DmtxEncode * enc;

    rb_scan_args(argc, argv, "13", &string,
                 &margin, &module, &size);

    safeString = StringValue(string);
    if(NIL_P(margin)) {
        safeMargin = 5;
    } else {
        safeMargin = NUM2INT(margin);
    }
    if(NIL_P(module)) {
        safeModule = 5;
    } else {
        safeModule = NUM2INT(module);
    }
    if(NIL_P(size)) {
        safeSize = DmtxSymbolSquareAuto;
    } else {
        safeSize = NUM2INT(size);
    }

    // printf("Margin = %d, Module = %d, Size = %d\n", safeMargin, safeModule, safeSize);

    /* Create and initialize libdmtx structures */
    enc = dmtxEncodeCreate();

    dmtxEncodeSetProp(enc, DmtxPropPixelPacking, DmtxPack24bppRGB);
    dmtxEncodeSetProp(enc, DmtxPropSizeRequest, safeSize);

    dmtxEncodeSetProp(enc, DmtxPropMarginSize, safeMargin);
    dmtxEncodeSetProp(enc, DmtxPropModuleSize, safeModule);

    /* Create barcode image */
    if (dmtxEncodeDataMatrix(enc, RSTRING_LEN(safeString),
            (unsigned char *)RSTRING_PTR(safeString)) == DmtxFail) {
//        printf("Fatal error !\n");
        dmtxEncodeDestroy(&enc);
        return Qnil;
    }

    width = dmtxImageGetProp(enc->image, DmtxPropWidth);
    height = dmtxImageGetProp(enc->image, DmtxPropHeight);

    magickImageClass = rb_path2class("Magick::Image");
    outputImage = rb_funcall(magickImageClass, rb_intern("new"), 2, INT2NUM(width), INT2NUM(height));

    rb_funcall(outputImage, rb_intern("import_pixels"), 7,
               INT2NUM(0),
               INT2NUM(0),
               INT2NUM(width),
               INT2NUM(height),
               rb_str_new("RGB", 3),
               rb_str_new((char *)enc->image->pxl, 3*width*height),
//               rb_const_get("Magick" ,rb_intern("CharPixel"))
               rb_eval_string("Magick::CharPixel"));

    /* Clean up */
    dmtxEncodeDestroy(&enc);

    return outputImage;
}

VALUE cRdmtx;
void Init_Rdmtx() {
    cRdmtx = rb_define_class("Rdmtx", rb_cObject);
    rb_define_method(cRdmtx, "initialize", rdmtx_init, 0);
    rb_define_method(cRdmtx, "decode", rdmtx_decode, 2);
    rb_define_method(cRdmtx, "encode", rdmtx_encode, -1);

    rb_define_global_const("DmtxSymbolRectAuto", INT2FIX(DmtxSymbolRectAuto));
    rb_define_global_const("DmtxSymbolSquareAuto", INT2FIX(DmtxSymbolSquareAuto));
    rb_define_global_const("DmtxSymbolShapeAuto", INT2FIX(DmtxSymbolShapeAuto));

    rb_define_global_const("DmtxSymbol10x10", INT2FIX(DmtxSymbol10x10));
    rb_define_global_const("DmtxSymbol12x12", INT2FIX(DmtxSymbol12x12));
    rb_define_global_const("DmtxSymbol14x14", INT2FIX(DmtxSymbol14x14));
    rb_define_global_const("DmtxSymbol16x16", INT2FIX(DmtxSymbol16x16));
    rb_define_global_const("DmtxSymbol18x18", INT2FIX(DmtxSymbol18x18));
    rb_define_global_const("DmtxSymbol20x20", INT2FIX(DmtxSymbol20x20));
    rb_define_global_const("DmtxSymbol22x22", INT2FIX(DmtxSymbol22x22));
    rb_define_global_const("DmtxSymbol24x24", INT2FIX(DmtxSymbol24x24));
    rb_define_global_const("DmtxSymbol26x26", INT2FIX(DmtxSymbol26x26));
    rb_define_global_const("DmtxSymbol32x32", INT2FIX(DmtxSymbol32x32));
    rb_define_global_const("DmtxSymbol36x36", INT2FIX(DmtxSymbol36x36));
    rb_define_global_const("DmtxSymbol40x40", INT2FIX(DmtxSymbol40x40));
    rb_define_global_const("DmtxSymbol44x44", INT2FIX(DmtxSymbol44x44));
    rb_define_global_const("DmtxSymbol48x48", INT2FIX(DmtxSymbol48x48));
    rb_define_global_const("DmtxSymbol52x52", INT2FIX(DmtxSymbol52x52));
    rb_define_global_const("DmtxSymbol64x64", INT2FIX(DmtxSymbol64x64));
    rb_define_global_const("DmtxSymbol72x72", INT2FIX(DmtxSymbol72x72));
    rb_define_global_const("DmtxSymbol80x80", INT2FIX(DmtxSymbol80x80));
    rb_define_global_const("DmtxSymbol88x88", INT2FIX(DmtxSymbol88x88));
    rb_define_global_const("DmtxSymbol96x96", INT2FIX(DmtxSymbol96x96));
    rb_define_global_const("DmtxSymbol104x104", INT2FIX(DmtxSymbol104x104));
    rb_define_global_const("DmtxSymbol120x120", INT2FIX(DmtxSymbol120x120));
    rb_define_global_const("DmtxSymbol132x132", INT2FIX(DmtxSymbol132x132));
    rb_define_global_const("DmtxSymbol144x144", INT2FIX(DmtxSymbol144x144));
    rb_define_global_const("DmtxSymbol8x18", INT2FIX(DmtxSymbol8x18));
    rb_define_global_const("DmtxSymbol8x32", INT2FIX(DmtxSymbol8x32));
    rb_define_global_const("DmtxSymbol12x26", INT2FIX(DmtxSymbol12x26));
    rb_define_global_const("DmtxSymbol12x36", INT2FIX(DmtxSymbol12x36));
    rb_define_global_const("DmtxSymbol16x36", INT2FIX(DmtxSymbol16x36));
    rb_define_global_const("DmtxSymbol16x48", INT2FIX(DmtxSymbol16x48));
}
