#version 450


layout(local_size_x = 16, local_size_y = 16) in;
layout(rgba32f, binding = 0) uniform image2D img_output;
uniform dvec2 pos;
uniform float zoom;
uniform int iteration;
uniform float power;
uniform int fractalType;
uniform dvec2 c;
uniform ivec3 rgb;

float h2 = 0.2;
float angle = 45;
vec2 v = { 0.70710678,0.70710678 };
int R = 100;
dvec2 powc(double x, double y, double p)
{
    double n = sqrt(x * x + y * y);
    double arg;
    
    if (y != 0 || x >= 0)
        arg = atan(float(y),float(x));
    else
        arg = 3.14;
    double a = p * arg;
    return dvec2(pow(float(n),float(p)) * cos(float(a)), pow(float(n),float(p)) * sin(float(a)));

}


void main() {
    int local_size_x = 16, local_size_y = 16;
    ivec2 coords = ivec2(gl_GlobalInvocationID);
    vec4 pixel;
    double x = (double(coords.x) / (1920 / 3.) - 1.5) / zoom - 0.5 + pos.x;
    double y = (double(coords.y) / (1080. / 2.) - 1.) / zoom + pos.y;
    double x1 = 0.;
    double y1 = 0.;
    
    
    double x2;
    /*float dcx = 1;
    float dcy = 0;
    float derx = dcx;
    float dery = dcy;*/
    pixel = vec4(.0, .0, .0, 1.0);
    if (fractalType == 0)
    {


        for (int i = 0; i < iteration; i++)
        {
            if (x1 * x1 + y1 * y1 > R * R)
            {
                
                pixel = vec4((((rgb.r * i) % 256) / 255.), (((rgb.g * i) % 256) / 255.), (((rgb.b * i) % 256) / 255.), 1.);;
                break;
            }

           
           

            dvec2 a = powc(x1, y1, power);
            x1 = x + a.x;
            y1 = y + a.y;
            


        }
    }
    else if (fractalType == 1)
    {

        for (int i = 0; i < iteration; i++)
        {
            if (x * x + y * y > R * R)
            {
                
                pixel = vec4((((rgb.r * i) % 256) / 255.), (((rgb.g * i) % 256) / 255.), (((rgb.b * i) % 256) / 255.), 1.);
                break;
            }

            x2 = c.x + x * x - y * y;
            y = c.y + 2. * x * y;
            x = x2;

           


        }
    }
    imageStore(img_output, coords, pixel);
}