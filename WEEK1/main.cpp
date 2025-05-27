#include <iostream>
#include "vec3.h"
using namespace std;

int main() {
    Vec3 v1(1.0f, -1.0f, 0.0f);
    Vec3 v2(4.0f, 5.0f, 6.0f);
    Vec3 v13(0.0f, 1.0f, 0.0f);
    float dp = v1.dot(v2);
    Vec3 v3 = v1.cross(v2);
    float l = v1.length();
    Vec3 v4 = v1.unit();
    Vec3 v5 = -v2;
    Vec3 v6 = v1 * 2;
    Vec3 v7 = v1 / 2;
    float el = v1[1];
    Vec3 v8 = v1.reflect(v13);
    Vec3 v10 = v1.refract(v13,1.414f);

    
    cout << "dot product" << dp<<endl;
    cout << "cross product" <<"("<<v3.x()<<","<<v3.y() <<","<< v3.z()<<")"<< endl;
    cout << "lenth " << l<<endl;
    cout<<"unit "<< "(" << v4.x() << "," << v4.y() << "," << v4.z() << ")" << endl;
    cout << "neg " << "(" << v5.x() << "," << v5.y() << "," << v5.z() << ")" << endl;
    cout << "pro " << "(" << v6.x() << "," << v6.y() << "," << v6.z() << ")" << endl;
    cout << "div " << "(" << v7.x() << "," << v7.y() << "," << v7.z() << ")" << endl;
    cout << "element " << el << endl;
    cout << "ref " << "(" << v8.x() << "," << v8.y() << "," << v8.z() << ")" << endl;
    cout << "refra " << "(" << v10.x() << "," << v10.y() << "," << v10.z() << ")" << endl;
    return 0;
}
