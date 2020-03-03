#include "camera.h"
//type , class:: variable(params){}
camera::camera()
{
    //set transforms not to zero 
   glm::vec3 m_up = glm::vec3(0, 1, 0);
   

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
glm::mat4 camera ::getMatrices()
{
   return m_projectionviewtransform = m_projectiontransform * m_viewtransform;
}
glm::vec3 camera::getWorldPos()
{
    return m_worldtransform[3];
}
glm::vec3 camera::getUpVec()
{
    return m_worldtransform[1]; 

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
void camera::updatef(float deltatime)
{
    //glfw get current context
   // glfwGetCurrentContext();
  //  glfwPollEvents();
   // glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);
    auto window = glfwGetCurrentContext();
    bool input_flag = false;
    glm::vec4 displacment = glm::vec4(0.0f);
    if (glfwGetKey(window, GLFW_KEY_W))
    {
        //printf("W was pressed"); // used for debugging 
        displacment -= m_worldtransform[2];
        input_flag = true;
    }

    if (glfwGetKey(window, GLFW_KEY_A))
    {
        //printf("A was pressed"); // used for debugging 
        displacment -= m_worldtransform[0];
        input_flag = true;
    }

    if (glfwGetKey(window, GLFW_KEY_S))
    {
      //  printf("S was pressed"); // used for debugging 
        displacment += m_worldtransform[2];
        input_flag = true;
    }

    if (glfwGetKey(window,GLFW_KEY_D))
    {
       // printf("D was pressed"); // used for debugging 
        displacment +=  m_worldtransform[0];
        input_flag = true;
    }
    if(input_flag)
    {
       // setPosition((getworldtransform()[3]) + (glm::vec4(glm::normalize( displacment)), 0.0f)* translation_speed * deltatime );
       // setPosition((getworldtransform(), 0.0f) + displacment * translation_speed * deltatime);
       // getMatrices();
        setPosition(glm::vec4 (getWorldPos(), 0.0f) + ((glm::normalize(displacment)/* 0.0f*/) * translation_speed * deltatime)); //this was creating a vec5 when it adds on 0.0f
        m_projectionviewtransform = m_projectiontransform * m_viewtransform; // pv = p * v
    }
    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1))
    {
        //mouse look
        double cursorPosx;
        double cursorPosy;
        //current cursor position
        glfwGetCursorPos(window, &cursorPosx, &cursorPosy);
        // calculate the offset from the screens center this frame
        double deltax = cursorPosx - (1280 * 0.5);
        double deltay = cursorPosy - (720 * 0.5);
        glfwSetCursorPos(window, 1280 * 0.5, 720 * 0.5);
        if(deltax || deltay)
        {
            auto rotation = glm::mat4(1.0f);
            //left /right
           // rotation = rotation * glm::rotate(float(angularSpeed * deltatime * -deltax), glm::vec3(m_worldtransform[1]));
            rotation =  glm::rotate(rotation,float(angularSpeed * deltatime * -deltax), glm::vec3(m_viewtransform[1]));
      
            //up/ down rotation
            rotation =  glm::rotate(rotation,float(angularSpeed * deltatime * -deltay), glm::vec3(1.0f, 0.0f, 0.0f));
          // apply the rotaion to the camera
            m_worldtransform = m_worldtransform * rotation;
            //preserve the inverse
            m_viewtransform = glm::inverse(m_worldtransform);
           //update the PV
           m_projectionviewtransform = m_projectiontransform * m_viewtransform; // pv = p * v
          // getMatrices();
        }
    }
}
void camera::setSpeed(float speed)
{
    
}
