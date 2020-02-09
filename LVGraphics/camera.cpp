#include "camera.h"
//type , class:: variable(params){}
camera::camera()
{

}
camera::~camera()
{

}
void camera::update(float deltaTime)
{
     deltaTime = 0;
}
glm::mat4 camera::getview() //get look at
{
    return m_viewtransform;
}
glm::mat4 camera::getworldtransform() //get position
{
    return m_worldtransform;
}
glm::mat4 camera ::getprojectiontransform() //get perspective
{
    return m_projectiontransform;
}
glm::mat4 camera ::getprojectionviewtransform()
{
    return m_projectionviewtransform;
}
void camera::setPosition(glm::vec3 pos)
{
    m_worldtransform[3] = glm::vec4(pos, 1);
    m_viewtransform = glm::inverse(m_worldtransform);
    m_projectionviewtransform = m_projectiontransform * m_viewtransform; // pv = p * v
    
}
void camera::setPerspective(float FOV, float aspectRatio, float a_near, float a_far)
{
   m_projectiontransform = glm::perspective(FOV, aspectRatio, a_near, a_far);
   m_projectionviewtransform = m_projectiontransform * m_viewtransform; // pv = p * v
   //check if right ^
}
void camera::setLookAt(glm::vec3 from, glm::vec3 point, glm::vec3 up)
{
   m_viewtransform = glm::lookAt(from, point, up);
   m_worldtransform = glm::inverse(m_viewtransform);
   m_projectionviewtransform = m_projectiontransform * m_viewtransform; // pv = p * v
}
void camera::setLookAtPoint(glm::vec3 point)
{
  //  m_viewtransform = setLookAt(point);
   //setLookAt(point);
    setLookAt(m_worldtransform[3], point, glm::vec3(0, 1, 0));
   // glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}