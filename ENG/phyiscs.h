#include <math.h>
#include <GLUT/glut.h>  // GLUT, include glu.h and gl.h

static float tick = 0;
static float gravity = 1;
static float friction = 0.8;

typedef struct Vec3 {
    float x;
    float y;
    float z;
};

typedef struct Vec2 {
    float x;
    float y;
};

typedef struct Velocity3d {
    Vec3 veloctiy3d;
};

typedef struct Velocity2d {
    Vec2 veloctiy2;
};

typedef struct SphericalPhysicsBody3d {
    Vec3 vec3;
    bool moveable = 1;
    Velocity3d velocity;
    float size;
};

typedef struct SphericalPhysicsBody2d {
    Vec2 vec2;
    bool moveable = 1;
    Velocity2d velocity;
    float size;
};

typedef struct BoxPhysicsBody3d {
    Vec3 vec3;
    bool moveable = 1;
    Velocity3d velocity;
    Vec3 size;
};

typedef struct BoxPhysicsBody2d {
    Vec2 vec2;
    bool moveable = 1;
    Velocity2d velocity;
    Vec2 size;
};


void Translatecamf (Vec3 campos) {
    glTranslatef(-campos.x, -campos.y, -campos.z);
}

void UseGravity3d(Vec3 vec3) {
    vec3.z -= tick*gravity*tick*gravity;
}

void UseGravity3d(Vec3 vec3, float gravitymultiplyier) {
    vec3.z -= tick*gravity*gravitymultiplyier*tick*gravity*gravitymultiplyier;
}

void UseGravity2d(Vec2 vec2) {
    vec2.y -= tick*gravity*tick*gravity;
}

void UseGravity3d(Vec2 vec2, float gravitymultiplyier) {
    vec2.y -= tick*gravity*gravitymultiplyier*tick*gravity*gravitymultiplyier;
}
float square(float FLOAT) {
    return FLOAT*FLOAT;
}

int Distance(SphericalPhysicsBody2d firstphysicsbody2d, SphericalPhysicsBody2d seccondphysicsbody2d, unsigned long predistance) {
    predistance = square(firstphysicsbody2d.vec2.x) + (seccondphysicsbody2d.vec2.x);
    predistance += square(firstphysicsbody2d.vec2.y) + (seccondphysicsbody2d.vec2.y);
    return sqrt(predistance);
}
int Distance(SphericalPhysicsBody3d firstphysicsbody3d, SphericalPhysicsBody3d seccondphysicsbody3d, unsigned long predistance) {
    predistance = square(firstphysicsbody3d.vec3.x) + (seccondphysicsbody3d.vec3.x);
    predistance += square(firstphysicsbody3d.vec3.y) + (seccondphysicsbody3d.vec3.y);
    predistance += square(firstphysicsbody3d.vec3.z) + (seccondphysicsbody3d.vec3.z);
    return sqrt(predistance);
}

void DetectBoxCollision(BoxPhysicsBody3d firstphysicsbody3d, BoxPhysicsBody3d seccondphysicsbody3d) {
    if (firstphysicsbody3d.vec3.x - seccondphysicsbody3d.vec3.x < firstphysicsbody3d.size.x + seccondphysicsbody3d.size.x) {
        if (firstphysicsbody3d.vec3.y - seccondphysicsbody3d.vec3.y < firstphysicsbody3d.size.y + seccondphysicsbody3d.size.y) {
            if (firstphysicsbody3d.vec3.z - seccondphysicsbody3d.vec3.z < firstphysicsbody3d.size.z + seccondphysicsbody3d.size.z) {
                firstphysicsbody3d.velocity.veloctiy3d.x = firstphysicsbody3d.velocity.veloctiy3d.x * -friction * seccondphysicsbody3d.moveable * firstphysicsbody3d.moveable;
                firstphysicsbody3d.velocity.veloctiy3d.y = firstphysicsbody3d.velocity.veloctiy3d.y * -friction * seccondphysicsbody3d.moveable * firstphysicsbody3d.moveable;
                firstphysicsbody3d.velocity.veloctiy3d.z = firstphysicsbody3d.velocity.veloctiy3d.z * -friction * seccondphysicsbody3d.moveable * firstphysicsbody3d.moveable;
            }   
        }
    }
}

void DetectSphericalCollision(SphericalPhysicsBody2d firstphysicsbody2d, SphericalPhysicsBody2d seccondphysicsbody2d) {
    if(Distance(firstphysicsbody2d, seccondphysicsbody2d, 0) <= firstphysicsbody2d.size) {
        firstphysicsbody2d.velocity.veloctiy2.x = firstphysicsbody2d.velocity.veloctiy2.x * -friction * seccondphysicsbody2d.moveable * firstphysicsbody2d.moveable;
        firstphysicsbody2d.velocity.veloctiy2.y = firstphysicsbody2d.velocity.veloctiy2.y * -friction * seccondphysicsbody2d.moveable * firstphysicsbody2d.moveable;

        seccondphysicsbody2d.velocity.veloctiy2.x = seccondphysicsbody2d.velocity.veloctiy2.x * -friction * seccondphysicsbody2d.moveable * firstphysicsbody2d.moveable;
        seccondphysicsbody2d.velocity.veloctiy2.y = seccondphysicsbody2d.velocity.veloctiy2.y * -friction * seccondphysicsbody2d.moveable * firstphysicsbody2d.moveable;
    }
    else if(Distance(firstphysicsbody2d, seccondphysicsbody2d, 0) <= seccondphysicsbody2d.size) {
        firstphysicsbody2d.velocity.veloctiy2.x = firstphysicsbody2d.velocity.veloctiy2.x * -friction * seccondphysicsbody2d.moveable * firstphysicsbody2d.moveable;
        firstphysicsbody2d.velocity.veloctiy2.y = firstphysicsbody2d.velocity.veloctiy2.y * -friction * seccondphysicsbody2d.moveable * firstphysicsbody2d.moveable;

        seccondphysicsbody2d.velocity.veloctiy2.x = seccondphysicsbody2d.velocity.veloctiy2.x * -friction * seccondphysicsbody2d.moveable * firstphysicsbody2d.moveable;
        seccondphysicsbody2d.velocity.veloctiy2.y = seccondphysicsbody2d.velocity.veloctiy2.y * -friction * seccondphysicsbody2d.moveable * firstphysicsbody2d.moveable;
    }
}

void DetectSphericalCollision(SphericalPhysicsBody3d firstphysicsbody3d, SphericalPhysicsBody3d seccondphysicsbody3d) {
    if(Distance(firstphysicsbody3d, seccondphysicsbody3d, 0) <= firstphysicsbody3d.size) {
        firstphysicsbody3d.velocity.veloctiy3d.x = firstphysicsbody3d.velocity.veloctiy3d.x * -friction * seccondphysicsbody3d.moveable * firstphysicsbody3d.moveable;
        firstphysicsbody3d.velocity.veloctiy3d.y = firstphysicsbody3d.velocity.veloctiy3d.y * -friction * seccondphysicsbody3d.moveable * firstphysicsbody3d.moveable;
        firstphysicsbody3d.velocity.veloctiy3d.z = firstphysicsbody3d.velocity.veloctiy3d.z * -friction * seccondphysicsbody3d.moveable * firstphysicsbody3d.moveable;

        seccondphysicsbody3d.velocity.veloctiy3d.x = seccondphysicsbody3d.velocity.veloctiy3d.x * -friction * seccondphysicsbody3d.moveable * firstphysicsbody3d.moveable;
        seccondphysicsbody3d.velocity.veloctiy3d.y = seccondphysicsbody3d.velocity.veloctiy3d.y * -friction * seccondphysicsbody3d.moveable * firstphysicsbody3d.moveable;
        seccondphysicsbody3d.velocity.veloctiy3d.z = seccondphysicsbody3d.velocity.veloctiy3d.z * -friction * seccondphysicsbody3d.moveable * firstphysicsbody3d.moveable;
    }
    else if(Distance(firstphysicsbody3d, seccondphysicsbody3d, 0) <= seccondphysicsbody3d.size) {
        firstphysicsbody3d.velocity.veloctiy3d.x = firstphysicsbody3d.velocity.veloctiy3d.x * -friction * seccondphysicsbody3d.moveable * firstphysicsbody3d.moveable;
        firstphysicsbody3d.velocity.veloctiy3d.y = firstphysicsbody3d.velocity.veloctiy3d.y * -friction * seccondphysicsbody3d.moveable * firstphysicsbody3d.moveable;
        firstphysicsbody3d.velocity.veloctiy3d.z = firstphysicsbody3d.velocity.veloctiy3d.z * -friction * seccondphysicsbody3d.moveable * firstphysicsbody3d.moveable;

        seccondphysicsbody3d.velocity.veloctiy3d.x = seccondphysicsbody3d.velocity.veloctiy3d.x * -friction * seccondphysicsbody3d.moveable * firstphysicsbody3d.moveable;
        seccondphysicsbody3d.velocity.veloctiy3d.y = seccondphysicsbody3d.velocity.veloctiy3d.y * -friction * seccondphysicsbody3d.moveable * firstphysicsbody3d.moveable;
        seccondphysicsbody3d.velocity.veloctiy3d.z = seccondphysicsbody3d.velocity.veloctiy3d.z * -friction * seccondphysicsbody3d.moveable * firstphysicsbody3d.moveable;
    }
}