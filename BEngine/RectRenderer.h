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
	int m_resolution_level;	// �ػ� ����
	int m_max_resolution_level;	// �ִ� �ػ� ����

	int m_client_width;	// Ŭ���̾�Ʈ ���� �ʺ�
	int m_client_height;	// Ŭ���̾�Ʈ ���� ����

	int m_ratio_width;	// Ŭ���̾�Ʈ �ʺ��� ��
	int m_ratio_height;	// Ŭ���̾�Ʈ ������ ��

	int m_buffer_width;	// �簢���� ����ϴ� ������ �ʺ�
	int m_buffer_height;	// �簢���� ����ϴ� ������ ����

	int m_rect_width;	// ���ۿ� ����ϴ� �簢���� �ʺ�
	int m_rect_height;	// ���ۿ� ����ϴ� �簢���� ����

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



	// ������ �Լ�

	// ���� �غ�
	void Prepare_Render() override final;
	// ����
	void Render(HDC hdc) override final;

	// ������Ʈ ������
	void Render_Object(Object& object);
	// ������ ������
	void Render_Polygon(Polygon_Info& polygon_info);
	// �簢�� ������
	void Render_Rect(Vertex_Info& vertex_info);



	// ���� �ʱ�ȭ

	// �� ���� �ʱ�ȭ
	void Clear_Back_Buffer();
	// z ���� �ʱ�ȭ
	void Clear_Z_Buffer();
};

