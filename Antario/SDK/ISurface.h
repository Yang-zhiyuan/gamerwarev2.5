#pragma once
#include "..\Utils\Color.h"
#include "..\Utils\Utils.h"
#include <array>
#include "../Utils/Math.h"


enum EFontFlags
{
	FONTFLAG_NONE,
	FONTFLAG_ITALIC = 0x001,
	FONTFLAG_UNDERLINE = 0x002,
	FONTFLAG_STRIKEOUT = 0x004,
	FONTFLAG_SYMBOL = 0x008,
	FONTFLAG_ANTIALIAS = 0x010,
	FONTFLAG_GAUSSIANBLUR = 0x020,
	FONTFLAG_ROTARY = 0x040,
	FONTFLAG_DROPSHADOW = 0x080,
	FONTFLAG_ADDITIVE = 0x100,
	FONTFLAG_OUTLINE = 0x200,
	FONTFLAG_CUSTOM = 0x400,
	FONTFLAG_BITMAP = 0x800,
};

enum gradient_type_t {
	horizontal,
	vertical
};

struct Vertex_t
{
	Vector2D	mPosition;
	Vector2D	m_TexCoord;

	Vertex_t() {}
	Vertex_t(const Vector2D &pos, const Vector2D &coord = Vector2D(0, 0))
	{
		mPosition = pos;
		m_TexCoord = coord;
	}
	void Init(const Vector2D &pos, const Vector2D &coord = Vector2D(0, 0))
	{
		mPosition = pos;
		m_TexCoord = coord;
	}
};

typedef  Vertex_t FontVertex_t;

class ISurface
{
public:
	void UnLockCursor()
	{
		typedef void(__thiscall* Fn)(void*);
		Utils::GetVFunc< Fn >(this, 66)(this);
	}

	void DrawSetColor(Color col)
	{
		typedef void(__thiscall* Fn)(PVOID, Color);
		return Utils::GetVFunc< Fn >(this, 14)(this, col);
	}

	void DrawSetColor(int r, int g, int b, int a)
	{
		typedef void(__thiscall* Fn)(PVOID, int, int, int, int);
		Utils::GetVFunc< Fn >(this, 15)(this, r, g, b, a);
	}

	void DrawFilledRect(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* Fn)(PVOID, int, int, int, int);
		Utils::GetVFunc< Fn >(this, 16)(this, x0, y0, x1, y1);
	}

	void DrawOutlinedRect(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* Fn)(PVOID, int, int, int, int);
		Utils::GetVFunc< Fn >(this, 18)(this, x0, y0, x1, y1);
	}

	void DrawLine(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* Fn)(PVOID, int, int, int, int);
		Utils::GetVFunc< Fn >(this, 19)(this, x0, y0, x1, y1);
	}

	void DrawPolyLine(int *x, int *y, int count)
	{
		typedef void(__thiscall* Fn)(PVOID, int *, int *, int);
		Utils::GetVFunc< Fn >(this, 20)(this, x, y, count);
	}

	void set_drawing_color(int r, int g, int b, int a = 255) {
		using original_fn = void(__thiscall*)(ISurface*, int, int, int, int);
		return (*(original_fn * *)this)[15](this, r, g, b, a);
	}
	void set_texture(int id) {
		using original_fn = void(__thiscall*)(ISurface*, int);
		return (*(original_fn * *)this)[38](this, id);
	}
	void draw_polygon(int n, Vertex_t* vertice, bool clip_vertices = true) {
		using original_fn = void(__thiscall*)(ISurface*, int, Vertex_t*, bool);
		return (*(original_fn * *)this)[106](this, n, vertice, clip_vertices);
	}

	void add_textured_polygon(int n, Vertex_t* vertice, int r, int g, int b, int a) {
		static int texture_id = CreateNewTextureID(true);
		static unsigned char buf[4] = { 255, 255, 255, 255 };
		set_drawing_color(r, g, b, a);
		set_texture(texture_id);
		draw_polygon(n, vertice);
	}

	void TexturedPolygon(int n, std::vector<Vertex_t> vertice, Color color)
	{
		static int texture_id = CreateNewTextureID(true); // 
		static unsigned char buf[4] = { 255, 255, 255, 255 };
		DrawSetTextureRGBA(texture_id, buf, 1, 1); //
		DrawSetColor(color); //
		DrawSetTexture(texture_id); //
		DrawTexturedPolygon(n, vertice.data()); //
	}

	void draw_filled_triangle(std::array< Vector2D, 3 >points, Color colour) {
		std::array< Vertex_t, 3 >vertices{ Vertex_t(points.at(0)), Vertex_t(points.at(1)), Vertex_t(points.at(2)) };
		add_textured_polygon(3, vertices.data(), colour.red, colour.green, colour.blue, colour.alpha);
	}

	void DrawSetTextFont(unsigned long font)
	{
		typedef void(__thiscall* Fn)(PVOID, unsigned long);
		Utils::GetVFunc< Fn >(this, 23)(this, font);
	}

	void DrawSetTextColor(int r, int g, int b, int a)
	{
		typedef void(__thiscall* Fn)(PVOID, int, int, int, int);
		Utils::GetVFunc< Fn >(this, 25)(this, r, g, b, a);
	}

	void DrawSetTextColor(Color col)
	{
		typedef void(__thiscall* Fn)(PVOID, Color);
		return Utils::GetVFunc< Fn >(this, 24)(this, col);
	}

	void DrawSetTextPos(int x, int y)
	{
		typedef void(__thiscall* Fn)(PVOID, int, int);
		Utils::GetVFunc< Fn >(this, 26)(this, x, y);
	}

	void DrawPrintText(const wchar_t* text, int textLen)
	{
		typedef void(__thiscall* Fn)(PVOID, const wchar_t*, int, int);
		Utils::GetVFunc< Fn >(this, 28)(this, text, textLen, 0);
	}

	void DrawSetTexture(int textureID)
	{
		typedef void(__thiscall* Fn)(PVOID, int);
		return Utils::GetVFunc< Fn >(this, 38)(this, textureID);
	}

	void DrawSetTextureRGBA(int textureID, unsigned char  const* colors, int w, int h)
	{
		typedef void(__thiscall* Fn)(PVOID, int, unsigned char  const*, int, int);
		return Utils::GetVFunc< Fn >(this, 37)(this, textureID, colors, w, h);
	}

	int CreateNewTextureID(bool procedural)
	{
		typedef int(__thiscall* Fn)(PVOID, bool);
		return Utils::GetVFunc< Fn >(this, 43)(this, procedural);
	}

	void DrawTexturedPolygon(int vtxCount, FontVertex_t *vtx, bool bClipVertices = true)
	{
		typedef void(__thiscall* Fn)(PVOID, int, FontVertex_t*, bool);
		return Utils::GetVFunc< Fn >(this, 106)(this, vtxCount, vtx, bClipVertices);
	}

	void polygon(int n, Vertex_t* vertice, bool clip_vertices = true) {
		using original_fn = void(__thiscall*)(void*, int, Vertex_t*, bool);
		return (*(original_fn * *)this)[106](this, n, vertice, clip_vertices);
	}

	unsigned long FontCreate()
	{
		typedef unsigned int(__thiscall* Fn)(PVOID);
		return Utils::GetVFunc< Fn >(this, 71)(this);
	}

	void SetFontGlyphSet(unsigned long font, const char* windowsFontName, int tall, int weight, int blur, int scanlines, int flags)
	{
		typedef void(__thiscall* Fn)(PVOID, unsigned long, const char*, int, int, int, int, int, int, int);
		Utils::GetVFunc< Fn >(this, 72)(this, font, windowsFontName, tall, weight, blur, scanlines, flags, 0, 0);
	}

	void ISurface::GetTextSize(DWORD font, const wchar_t *text, int &wide, int &tall)
	{
		typedef void(__thiscall* Fn)(void*, DWORD, const wchar_t*, int&, int&);
		return Utils::GetVFunc<Fn>(this, 79)(this, font, text, wide, tall);
	}
	   
	void DrawOutlinedCircle(int x, int y, int r, int seg)
	{
		typedef void(__thiscall* Fn)(PVOID, int, int, int, int);
		return Utils::GetVFunc< Fn >(this, 103)(this, x, y, r, seg);
	}

	void DrawBruhCircle(int x, int y, int radius, Color c)
	{
		DrawSetColor(c);
		DrawOutlinedCircle(x, y, radius, 2000);
	}

	void DrawCircle(float x, float y, float r, float segments, Color color)
	{
	
		DrawSetColor(color);
		DrawOutlinedCircle(x, y, r, segments);
	}

	void SurfaceGetCursorPos(int &x, int &y)
	{
		typedef void(__thiscall* Fn)(PVOID, int&, int&);
		return Utils::GetVFunc< Fn >(this, 66)(this, x, y);
	}

	auto draw_gradient_rect(int x, int y, int w, int h, bool reversed, float alpha, int type) {
		using original_fn = void(__thiscall*)(ISurface*, int, int, int, int, unsigned int, unsigned int, bool);
		return (*(original_fn * *)this)[123](this, x, y, x + w, y + h, reversed ? alpha : 0, reversed ? 0 : alpha, type == 0);
	}

	void  add_gradient_rect(int x, int y, int w, int h, Color first, Color second, gradient_type_t type) {
		first.alpha = (static_cast<int>(first.alpha));
		second.alpha = (static_cast<int>(second.alpha));

		static auto blend = [](const Color & first, const Color & second, float t) -> Color {
			return Color(
				first.red + t * (second.red - first.red),
				first.green + t * (second.green - first.green),
				first.blue + t * (second.blue -first.blue),
				first.alpha + t * (second.alpha - first.alpha));
		};

		if (first.alpha == 255 || second.alpha == 255) {
			DrawSetColor(blend(first, second, 0.5f).red, blend(first, second, 0.5f).green, blend(first, second, 0.5f).blue, blend(first, second, 0.5f).alpha);
			DrawFilledRect(x, y, w, h);
		}

		DrawSetColor(first.red, first.green, first.blue, first.alpha);
		draw_gradient_rect(x, y, w, h, true, first.alpha, type);

		DrawSetColor(second.red, second.green, second.blue, second.alpha);
		draw_gradient_rect(x, y, w, h, false, second.alpha, type);
	}

	void RoundedFilledRect(int x, int y, int width, int height, float radius, Color col) { // UC https://www.unknowncheats.me/forum/1498179-post4.html
		// TODO: make the quality not hardcoded -green
		// don't you have to give it quality in the formula you wrote? 8 + 4 * ( quality ) ? -dex
		constexpr int quality = 24;

		static Vertex_t verts[quality];

		Vector2D add = { 0, 0 };

		for (int i = 0; i < quality; i++) {
			float angle = (static_cast < float > (i) / -quality) * 6.28f - (6.28f / 16.f);

			verts[i].mPosition.x = radius + x + add.x + (radius * sin(angle));
			verts[i].mPosition.y = height - radius + y + add.y + (radius * cos(angle));

			if (i == 4) {
				add.y = -height + (radius * 2);
			}
			else if (i == 10) {
				add.x = width - (radius * 2);
			}
			else if (i == 16) {
				add.y = 0;
			}
			else if (i == 22) {
				add.x = 0;
			}
		}
		DrawSetColor(col.red, col.green, col.blue, col.alpha);
		DrawTexturedPolygon(quality, verts);
	}

	void FilledRect(int x, int y, int w, int h, Color color)
	{
		DrawSetColor(color.red, color.green, color.blue, color.alpha);
		DrawFilledRect(x, y, x + w, y + h);
	}

	void FloatFilledRect(float x, float y, float w, float h, Color color)
	{
		DrawSetColor(color.red, color.green, color.blue, color.alpha);
		DrawFilledRect(x, y, x + w, y + h);
	}

	void OutlinedRect(int x, int y, int w, int h, Color color)
	{
		DrawSetColor(color.red, color.green, color.blue, color.alpha);
		DrawOutlinedRect(x, y, x + w, y + h);
	}

	void OutlinedVecRect(int x, int y, int x2, int y2, Color color)
	{
		DrawSetColor(color.red, color.green, color.blue, color.alpha);
		DrawLine(x, y, x2, y);
		DrawLine(x2, y, x2, y2);
		DrawLine(x2, y2, x, y2);
		DrawLine(x, y2, x, y);
	}

	void Line(int x, int y, int x2, int y2, Color color)
	{
		DrawSetColor(color.red, color.green, color.blue, color.alpha);
		DrawLine(x, y, x2, y2);
	}

	void DrawCircleFast(float x, float y, int radius, int numSides, Color color)
	{
		float step = (float)(PI * 2.0 / numSides);

		if (step <= 0)
			return;

		for (float a = 0; a < PI * 2.0; a += step)
		{
			float X1 = radius * cosf(a) + x;
			float Y1 = radius * sinf(a) + y;
			float X2 = radius * cosf(a + step) + x;
			float Y2 = radius * sinf(a + step) + y;

			Line(X1, Y1, X2, Y2, color);
		}
	}
	void FilledCircle(Vector2D position, float points, float radius, Color col)
	{
		std::vector<Vertex_t> vertices;
		float step = (float)M_PI * 2.0f / points;

		for (float a = 0; a < (M_PI * 2.0f); a += step)
			vertices.push_back(Vertex_t(Vector2D(radius * cosf(a) + position.x, radius * sinf(a) + position.y)));

		DrawSetColor(col.red, col.green, col.blue, col.alpha);
		DrawTexturedPolygon((int)points, vertices.data(), true);
	}

	void DrawT(int X, int Y, Color Color, int Font, bool Center, const char* _Input, ...)
	{
		int apple = 0;
		char Buffer[2048] = { '\0' };
		va_list Args;

		va_start(Args, _Input);
		vsprintf_s(Buffer, _Input, Args);
		va_end(Args);

		size_t Size = strlen(Buffer) + 1;
		wchar_t* WideBuffer = new wchar_t[Size];

		mbstowcs_s(0, WideBuffer, Size, Buffer, Size - 1);

		int Width = 0, Height = 0;

		if (Center)
			GetTextSize(Font, WideBuffer, Width, Height);

		DrawSetTextColor(Color.red, Color.green, Color.blue, Color.alpha);
		DrawSetTextFont(Font);
		DrawSetTextPos(X - (Width / 2), Y);
		DrawPrintText(WideBuffer, wcslen(WideBuffer));
	}

	RECT GetTextSizeRect(DWORD font, const char* text) // ayyware or something
	{
		size_t origsize = strlen(text) + 1;
		const size_t newsize = 100;
		size_t convertedChars = 0;
		wchar_t wcstring[newsize];
		mbstowcs_s(&convertedChars, wcstring, origsize, text, _TRUNCATE);

		RECT rect; int x, y;
		GetTextSize(font, wcstring, x, y);
		rect.left = x; rect.bottom = y;
		rect.right = x;
		return rect;
	}

	Vector2D GetMousePosition() // bolbi ware
	{
		POINT mousePosition;
		GetCursorPos(&mousePosition);
		ScreenToClient(FindWindow(0, "Counter-Strike: Global Offensive"), &mousePosition);
		return { static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y) };
	}

	bool MouseInRegion(int x, int y, int x2, int y2) {
		if (GetMousePosition().x > x && GetMousePosition().y > y && GetMousePosition().x < x2 + x && GetMousePosition().y < y2 + y)
			return true;
		return false;
	}
};

extern ISurface* g_pSurface;