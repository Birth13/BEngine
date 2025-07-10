#pragma once
#include "common.h"

#include "Renderer.h"

struct Mesh_Info;
struct Polygon_Info;
struct Vertex_Info;

class Object;

class RectRenderer : public Renderer {
private:
	//
	int m_resolution_level;	// 해상도 레벨
	int m_max_resolution_level;	// 최대 해상도 레벨

	int m_client_width;	// 클라이언트 영역 너비
	int m_client_height;	// 클라이언트 영역 높이

	int m_ratio_width;	// 클라이언트 너비의 비
	int m_ratio_height;	// 클라이언트 높이의 비

	int m_buffer_width;	// 사각형을 출력하는 버퍼의 너비
	int m_buffer_height;	// 사각형을 출력하는 버퍼의 높이

	int m_rect_width;	// 버퍼에 출력하는 사각형의 너비
	int m_rect_height;	// 버퍼에 출력하는 사각형의 높이

	//
	HDC m_memory_dc_handle;
	HBITMAP m_bitmap_handle;
	HBITMAP m_old_bitmap_handle;

	//
	UINT* m_back_buffer = nullptr;
	std::vector<float> m_z_buffer;

public :
	RectRenderer(std::wstring window_name, int resolution_level);
	~RectRenderer() override final;



	// 렌더링 함수

	// 렌더 준비
	void Prepare_Render() override final;
	// 렌더
	void Render(HDC hdc) override final;

	// 오브젝트 렌더링
	void Render_Object(Object& object);
	// 폴리곤 렌더링
	void Render_Polygon(Polygon_Info& polygon_info);
	// 사각형 렌더링
	void Render_Rect(Vertex_Info& vertex_info);



	// 버퍼 초기화

	// 백 버퍼 초기화
	void Clear_Back_Buffer();
	// z 버퍼 초기화
	void Clear_Z_Buffer();
};

