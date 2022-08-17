

//=============================================================================
// 원형 충돌 감지 메소드
// 기본 충돌 감지 메소드인 collision()에서 호출한다.
// Post: 충돌한 경우 true, 아니면 false 반환
//       충돌한 경우 collisionVector를 설정한다.
//=============================================================================
bool Entity::collideCircle(Entity& ent, VECTOR2& collisionVector)
{
    // 두 중심 차이의 차이
    distSquared = *getCenter() - *ent.getCenter();
    distSquared.x = distSquared.x * distSquared.x;      // difference squared
    distSquared.y = distSquared.y * distSquared.y;

    // 각 원의 반지름 합을 계산한다.
    sumRadiiSquared = (radius * getScale()) + (ent.radius * ent.getScale());
    sumRadiiSquared *= sumRadiiSquared;                 // square it

    // 개체가 충돌한다면
    if (distSquared.x + distSquared.y <= sumRadiiSquared)
    {
        // 충돌 벡터 설정한다. 
        collisionVector = *ent.getCenter() - *getCenter();
        return true;
    }
    return false;  
}

//=============================================================================
// 축 정렬 경계 상자(Axis-Aligned Bounding Box) 충돌 감지 메소드
// collision()에서 호출한다. 
// Post: 충돌한 경우 true, 아닌 경우 false를 반환한다.
//       충돌한 경우 collisionVector를 설정한다.
//=============================================================================
bool Entity::collideBox(Entity& ent, VECTOR2& collisionVector)
{
    // 개체 중 하나가 활성 상태가 아니라면 충돌이 일어나지 않는다. 
    if (!active || !ent.getActive())
        return false;

    // 축 정렬 경계 상자를 사용해 충돌을 검사한다. 
    if ((getCenterX() + edge.right * getScale() >= ent.getCenterX() + ent.getEdge().left * ent.getScale()) &&
        (getCenterX() + edge.left * getScale() <= ent.getCenterX() + ent.getEdge().right * ent.getScale()) &&
        (getCenterY() + edge.bottom * getScale() >= ent.getCenterY() + ent.getEdge().top * ent.getScale()) &&
        (getCenterY() + edge.top * getScale() <= ent.getCenterY() + ent.getEdge().bottom * ent.getScale()))
    {
        // 충돌벡터를 설정한다. 
        collisionVector = *ent.getCenter() - *getCenter();
        return true;
    }
    return false;
}

//=============================================================================
// 회전된 상자형 충돌 감지 메소드
// Called by collision()
// Post: 충돌한 경우 true, 아닌 경우 false를 반환한다. 
//       충돌한 경우 collisionVector를 설정한다. 
// 충돌을 감지하기 위해 분리된 축 검사를 사용한다. 
// 분리된 축 검사: 두 상자를 선에 투영시켰을 때 겹치지 않는다면 충돌하지 않는다. 
//=============================================================================
bool Entity::collideRotatedBox(Entity& ent, VECTOR2& collisionVector)
{

    computeRotatedBox();                    // prepare rotated box
    ent.computeRotatedBox();                // prepare rotated box
    if (projectionsOverlap(ent) && ent.projectionsOverlap(*this))
    {
        // set collision vector
        collisionVector = *ent.getCenter() - *getCenter();
        return true;
    }
    return false;
}

//=============================================================================
// 다른 상자를 edge01과 edge03에 투영한다. 
// Called by collideRotatedBox()
// Post: 투영선이 겹치는 경우 true, 겹치지 않는 경우 false를 반환한다. 
//=============================================================================
bool Entity::projectionsOverlap(Entity& ent)
{
    float projection, min01, max01, min03, max03;

    // 다른 상자를 edge01에 투영한다. 
    projection = graphics->Vector2Dot(&edge01, ent.getCorner(0)); // project corner 0
    min01 = projection;
    max01 = projection;

    // 모서리 0을 투영한다. 
    for (int c = 1; c < 4; c++)
    {
        // 모서리를 edge01에 투영한다. 
        projection = graphics->Vector2Dot(&edge01, ent.getCorner(c));
        if (projection < min01)
            min01 = projection;
        else if (projection > max01)
            max01 = projection;
    }
    //투영선이 겹치지 않는다면
    if (min01 > edge01Max || max01 < edge01Min) // if projections do not overlap
        return false;                       // no collision is possible

    // 다른 상자를 edge03에 투영한다. 
    projection = graphics->Vector2Dot(&edge03, ent.getCorner(0)); // project corner 0
    min03 = projection;
    max03 = projection;

    // 나머지 모서리에 대해서 
    for (int c = 1; c < 4; c++)
    {
        // 모서리를 edge03에 투영한다. 
        projection = graphics->Vector2Dot(&edge03, ent.getCorner(c));
        if (projection < min03)
            min03 = projection;
        else if (projection > max03)
            max03 = projection;
    }
    if (min03 > edge03Max || max03 < edge03Min) // if projections do not overlap
        return false;                       // no collision is possible

    return true;                            // projections overlap
}


//=============================================================================
// 회전된 상자의 모서리, 투영된 모서리와 투영선의 최솟값 및 최댓값을 계산한다. 
// 0---1  모서리 번호 
// |   |
// 3---2
//=============================================================================
void Entity::computeRotatedBox()
{
    if (rotatedBoxReady)
        return;
    float projection;

    VECTOR2 rotatedX(cos(spriteData.angle), sin(spriteData.angle));
    VECTOR2 rotatedY(-sin(spriteData.angle), cos(spriteData.angle));

    const VECTOR2* center = getCenter();
    corners[0] = *center + rotatedX * ((float)edge.left * getScale()) +
        rotatedY * ((float)edge.top * getScale());
    corners[1] = *center + rotatedX * ((float)edge.right * getScale()) +
        rotatedY * ((float)edge.top * getScale());
    corners[2] = *center + rotatedX * ((float)edge.right * getScale()) +
        rotatedY * ((float)edge.bottom * getScale());
    corners[3] = *center + rotatedX * ((float)edge.left * getScale()) +
        rotatedY * ((float)edge.bottom * getScale());

    // corners[0] 은 원점으로 사용됨.
    // corners[0]에 연결된 두 모서리는 투영선으로 사용됨.
    edge01 = VECTOR2(corners[1].x - corners[0].x, corners[1].y - corners[0].y);
    graphics->Vector2Normalize(&edge01);
    edge03 = VECTOR2(corners[3].x - corners[0].x, corners[3].y - corners[0].y);
    graphics->Vector2Normalize(&edge03);
     
    // 이 개체는 모서리로 투영한 결과의 최솟값과 최댓값이다. 
    projection = graphics->Vector2Dot(&edge01, &corners[0]);
    edge01Min = projection;
    edge01Max = projection;

    // edge01으로 투영한다. 
    projection = graphics->Vector2Dot(&edge01, &corners[1]);
    if (projection < edge01Min)
        edge01Min = projection;
    else if (projection > edge01Max)
        edge01Max = projection;

    // edge03으로 투영한다. 
    projection = graphics->Vector2Dot(&edge03, &corners[0]);
    edge03Min = projection;
    edge03Max = projection;

    projection = graphics->Vector2Dot(&edge03, &corners[3]);
    if (projection < edge03Min)
        edge03Min = projection;
    else if (projection > edge03Max)
        edge03Max = projection;

    rotatedBoxReady = true;
}


