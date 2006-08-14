/*  Copyright 2006 Guillaume Duhamel
    Copyright 2006 Fabien Coulon

    This file is part of Yabause.

    Yabause is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    Yabause is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Yabause; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "gtkglwidget.h"
#include <gtk/gtkgl.h>

int draw(GtkWidget * glxarea) {
	GdkGLContext *glcontext = gtk_widget_get_gl_context (glxarea);
	GdkGLDrawable *gldrawable = gtk_widget_get_gl_drawable (glxarea);

	if (!gdk_gl_drawable_make_current (gldrawable, glcontext)) {
		g_print("Cannot set gl drawable current\n");
		return;
	}

	gdk_gl_drawable_swap_buffers(gldrawable);
}

int drawPause(GtkWidget * glxarea) {

  gdk_draw_rectangle(glxarea->window,
		     glxarea->style->bg_gc[GTK_WIDGET_STATE (glxarea)],
		     TRUE,
		     0, 0, glxarea->allocation.width, glxarea->allocation.height);
}

static gboolean resize (GtkWidget *w,GdkEventConfigure *event, gpointer data) {
	GdkGLContext *glcontext = gtk_widget_get_gl_context (w);
	GdkGLDrawable *gldrawable = gtk_widget_get_gl_drawable (w);

	if (!gdk_gl_drawable_gl_begin (gldrawable, glcontext))
		return FALSE;

	glViewport(0, 0, event->width, event->height);

	return FALSE;
}

GtkWidget * gtk_gl_widget_new(void) {
	GtkWidget * drawingArea;
	int attribs[] = {
		GDK_GL_RGBA,
		GDK_GL_RED_SIZE,   1,
		GDK_GL_GREEN_SIZE, 1,
		GDK_GL_BLUE_SIZE,  1,

		GDK_GL_DOUBLEBUFFER,

		GDK_GL_DEPTH_SIZE ,1,

		GDK_GL_ATTRIB_LIST_NONE 
	};

	drawingArea = gtk_drawing_area_new();

	gtk_widget_set_gl_capability(drawingArea, gdk_gl_config_new(attribs), NULL, TRUE, GDK_GL_RGBA_TYPE);

	g_signal_connect (GTK_OBJECT(drawingArea),"configure_event", GTK_SIGNAL_FUNC(resize),0);

	return drawingArea;
}