#include "GameLevel.h"
#include "Errors.h"
#include "Json.h"
#include "ResourceManager.h"
#include "Texture.h"

#include <fstream>
#include <sstream>
#include <string.h>
#include <iostream>

Error GameLevel::load(const GLchar *file, GLuint levelWidth, GLuint levelHeight)
{
	JSON_Value* level = nullptr;

	CHECK_ERR(!file, ERR_INV_PAR);
	CHECK_ERR(!(level = json_parse_file(file)), ERR_INV_VAL);

    this->Bricks.clear();
	CHECK(this->init(level, levelWidth, levelHeight));
	json_value_free(level);
	return ERR_NO_ERR;
}

void GameLevel::draw(SpriteRenderer &renderer)
{
    for (GameObject &tile : this->Bricks) {
        if (!tile.destroyed) {
            tile.draw(renderer);
		}
	}
}

GLboolean GameLevel::isCompleted()
{
    for (GameObject &tile : this->Bricks) {
        if (!tile.isSolid && !tile.destroyed) {
            return GL_FALSE;
		}
	}
    return GL_TRUE;
}

Error GameLevel::init(JSON_Value* level, GLuint levelWidth, GLuint levelHeight)
{
	JSON_Object* root = nullptr;
	JSON_Array* tilesRows = nullptr, *tilesColumn = nullptr;
	GLuint rows = 0, columns = 0;
	GLfloat tileHeight = 0;

	CHECK_ERR(!level, ERR_INV_PAR);
	CHECK_ERR(!(root = json_value_get_object(level)), ERR_INV_VAL);
	CHECK_ERR(!(tilesRows = json_object_get_array(root, "tiles")), ERR_INV_VAL);

	rows = json_array_get_count(tilesRows);
	tileHeight = levelHeight/rows;
	GLfloat columnsWidth[rows];

	for (GLuint y = 0 ; y < rows ; y++) {
		CHECK_ERR(!(tilesColumn = json_array_get_array(tilesRows, y)), ERR_INV_VAL);
		columns = json_array_get_count(tilesColumn);
		columnsWidth[y] = (GLfloat)levelWidth/(GLfloat)columns;
		for (GLuint x = 0 ; x < columns ; x++) {
			GLuint isSolid = 0, visible = 0;
			GLfloat red = 0.0f, green = 0.0f, blue = 0.0f;
			JSON_Object* color = nullptr;
			JSON_Object* tile = json_array_get_object(tilesColumn, x);
			CHECK_ERR(!tile, ERR_INV_PAR);

			color = json_object_get_object(tile, "color");
			CHECK_ERR(!color, ERR_INV_VAL);

			isSolid = json_object_get_boolean(tile, "solid");
			visible = json_object_get_boolean(tile, "visible");
			red = json_object_get_number(color, "red");
			green = json_object_get_number(color, "green");
			blue = json_object_get_number(color, "blue");

			glm::vec2 pos(columnsWidth[y] * x, tileHeight * y);
			glm::vec2 size(columnsWidth[y], tileHeight);
			glm::vec3 tileColor = glm::vec3(1.0f);
			
			if (visible) {
				if (isSolid) {
					Texture2D* solidBlock = ResourceManagerGetTexture(RESOURCE_MANAGER_TEXTURE2D_SOLID_BLOCK);
					CHECK_ERR(!solidBlock, ERR_INV_VAL);
					GameObject obj(pos, size, *solidBlock, glm::vec3{1.0}, glm::vec2{0,0});
					obj.isSolid = GL_TRUE;
					this->Bricks.push_back(obj);
				} else {
					Texture2D* block = ResourceManagerGetTexture(RESOURCE_MANAGER_TEXTURE2D_BLOCK);
					CHECK_ERR(!block, ERR_INV_VAL);
					tileColor = glm::vec3(red, green, blue);
					GameObject obj(pos, size, *block, tileColor, glm::vec2{0,0});
					this->Bricks.push_back(obj);
				}
			}
		}
	}
	return ERR_NO_ERR;
}
