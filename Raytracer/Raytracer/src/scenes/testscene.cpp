#include "precomp.h"
#include "scenes/testscene.h"

testscene::testscene() : scene()
{

	
	m_prPrimitives[0] = new plane{ vec3(0.f, -1.5f, 0.f), vec3(0.f,1.f,0.f), 1.5f };
	
	m_prPrimitives[1] = new sphere{ vec3(0.f, 0.f, -3.f), 1.0f };
	m_prPrimitives[1]->m_maObjectMaterial = { {1.f,0.f,1.f}, 0.7f, 1.5f };
	
	m_prPrimitives[2] = new sphere{ vec3(2.f, -0.5f,-5.f), 1.f };
	m_prPrimitives[2]->m_maObjectMaterial = { {1.f,0.f,0.f}, 0.5f, 1.5f };

	m_prPrimitives[3] = new sphere{ vec3(-2.5f, 3.f,-8.f), 1.f };
	m_prPrimitives[3]->m_maObjectMaterial = { {1.f,1.f,0.f}, 1.0f, 1.5f };

	m_prPrimitives[4] = new sphere{ vec3(0.5f, -1.1f,-2.5f), 0.2f };
	m_prPrimitives[4]->m_maObjectMaterial = { {1.f,1.f,0.f}, 0.0f, 1.5f };

	m_prPrimitives[5] = new plane{{0.f,0.f,0.f}, {0.5f,1.f,0.f}, 2};
	m_prPrimitives[5]->m_maObjectMaterial = { {}, 0.5f, 1.5f };

	m_iPrimitiveCounter = 6;

	m_lsLight = {{4.0f, 10.f, 3.0f}, {}, {200.f,200.f,200.f} };
}
