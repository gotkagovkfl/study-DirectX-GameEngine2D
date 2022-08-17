

//=============================================================================
// ���� �浹 ���� �޼ҵ�
// �⺻ �浹 ���� �޼ҵ��� collision()���� ȣ���Ѵ�.
// Post: �浹�� ��� true, �ƴϸ� false ��ȯ
//       �浹�� ��� collisionVector�� �����Ѵ�.
//=============================================================================
bool Entity::collideCircle(Entity& ent, VECTOR2& collisionVector)
{
    // �� �߽� ������ ����
    distSquared = *getCenter() - *ent.getCenter();
    distSquared.x = distSquared.x * distSquared.x;      // difference squared
    distSquared.y = distSquared.y * distSquared.y;

    // �� ���� ������ ���� ����Ѵ�.
    sumRadiiSquared = (radius * getScale()) + (ent.radius * ent.getScale());
    sumRadiiSquared *= sumRadiiSquared;                 // square it

    // ��ü�� �浹�Ѵٸ�
    if (distSquared.x + distSquared.y <= sumRadiiSquared)
    {
        // �浹 ���� �����Ѵ�. 
        collisionVector = *ent.getCenter() - *getCenter();
        return true;
    }
    return false;  
}

//=============================================================================
// �� ���� ��� ����(Axis-Aligned Bounding Box) �浹 ���� �޼ҵ�
// collision()���� ȣ���Ѵ�. 
// Post: �浹�� ��� true, �ƴ� ��� false�� ��ȯ�Ѵ�.
//       �浹�� ��� collisionVector�� �����Ѵ�.
//=============================================================================
bool Entity::collideBox(Entity& ent, VECTOR2& collisionVector)
{
    // ��ü �� �ϳ��� Ȱ�� ���°� �ƴ϶�� �浹�� �Ͼ�� �ʴ´�. 
    if (!active || !ent.getActive())
        return false;

    // �� ���� ��� ���ڸ� ����� �浹�� �˻��Ѵ�. 
    if ((getCenterX() + edge.right * getScale() >= ent.getCenterX() + ent.getEdge().left * ent.getScale()) &&
        (getCenterX() + edge.left * getScale() <= ent.getCenterX() + ent.getEdge().right * ent.getScale()) &&
        (getCenterY() + edge.bottom * getScale() >= ent.getCenterY() + ent.getEdge().top * ent.getScale()) &&
        (getCenterY() + edge.top * getScale() <= ent.getCenterY() + ent.getEdge().bottom * ent.getScale()))
    {
        // �浹���͸� �����Ѵ�. 
        collisionVector = *ent.getCenter() - *getCenter();
        return true;
    }
    return false;
}

//=============================================================================
// ȸ���� ������ �浹 ���� �޼ҵ�
// Called by collision()
// Post: �浹�� ��� true, �ƴ� ��� false�� ��ȯ�Ѵ�. 
//       �浹�� ��� collisionVector�� �����Ѵ�. 
// �浹�� �����ϱ� ���� �и��� �� �˻縦 ����Ѵ�. 
// �и��� �� �˻�: �� ���ڸ� ���� ���������� �� ��ġ�� �ʴ´ٸ� �浹���� �ʴ´�. 
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
// �ٸ� ���ڸ� edge01�� edge03�� �����Ѵ�. 
// Called by collideRotatedBox()
// Post: �������� ��ġ�� ��� true, ��ġ�� �ʴ� ��� false�� ��ȯ�Ѵ�. 
//=============================================================================
bool Entity::projectionsOverlap(Entity& ent)
{
    float projection, min01, max01, min03, max03;

    // �ٸ� ���ڸ� edge01�� �����Ѵ�. 
    projection = graphics->Vector2Dot(&edge01, ent.getCorner(0)); // project corner 0
    min01 = projection;
    max01 = projection;

    // �𼭸� 0�� �����Ѵ�. 
    for (int c = 1; c < 4; c++)
    {
        // �𼭸��� edge01�� �����Ѵ�. 
        projection = graphics->Vector2Dot(&edge01, ent.getCorner(c));
        if (projection < min01)
            min01 = projection;
        else if (projection > max01)
            max01 = projection;
    }
    //�������� ��ġ�� �ʴ´ٸ�
    if (min01 > edge01Max || max01 < edge01Min) // if projections do not overlap
        return false;                       // no collision is possible

    // �ٸ� ���ڸ� edge03�� �����Ѵ�. 
    projection = graphics->Vector2Dot(&edge03, ent.getCorner(0)); // project corner 0
    min03 = projection;
    max03 = projection;

    // ������ �𼭸��� ���ؼ� 
    for (int c = 1; c < 4; c++)
    {
        // �𼭸��� edge03�� �����Ѵ�. 
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
// ȸ���� ������ �𼭸�, ������ �𼭸��� �������� �ּڰ� �� �ִ��� ����Ѵ�. 
// 0---1  �𼭸� ��ȣ 
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

    // corners[0] �� �������� ����.
    // corners[0]�� ����� �� �𼭸��� ���������� ����.
    edge01 = VECTOR2(corners[1].x - corners[0].x, corners[1].y - corners[0].y);
    graphics->Vector2Normalize(&edge01);
    edge03 = VECTOR2(corners[3].x - corners[0].x, corners[3].y - corners[0].y);
    graphics->Vector2Normalize(&edge03);
     
    // �� ��ü�� �𼭸��� ������ ����� �ּڰ��� �ִ��̴�. 
    projection = graphics->Vector2Dot(&edge01, &corners[0]);
    edge01Min = projection;
    edge01Max = projection;

    // edge01���� �����Ѵ�. 
    projection = graphics->Vector2Dot(&edge01, &corners[1]);
    if (projection < edge01Min)
        edge01Min = projection;
    else if (projection > edge01Max)
        edge01Max = projection;

    // edge03���� �����Ѵ�. 
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


