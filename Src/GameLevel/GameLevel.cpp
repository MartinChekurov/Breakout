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

void GameLevel::reset()
{
    for (GameObject &tile : this->Bricks) {
		tile.destroyed = false;
	}
}

GLboolean GameLevel::isCompleted()
{
    for (GameObject &tile : this->Bricks) {
        if (!tile.destroyed) {
            return GL_FALSE;
		}
	}
    return GL_TRUE;
}

Error GameLevel::init(JSON_Value* level, GLuint levelWidth, GLuint levelHeight)
{
	JSON_Object* root = nullptr, *dimensions = nullptr, *padding = nullptr;
	JSON_Array* tilesRows = nullptr, *tilesColumn = nullptr;
	GLuint rows = 0, columns = 0, left = 0, right = 0, up = 0, down = 50, xoffset = 0, yoffset = 0;
	GLfloat tileHeight = 0;

	CHECK_ERR(!level, ERR_INV_PAR);
	CHECK_ERR(!(root = json_value_get_object(level)), ERR_INV_VAL);
	
	dimensions = json_object_get_object(root, "dimensions");
	if (dimensions) {
		padding = json_object_get_object(dimensions, "padding");
		if (padding) {
			left = json_object_get_number(padding, "left");
			right = json_object_get_number(padding, "right");
			up = json_object_get_number(padding, "up");
			down = json_object_get_number(padding, "down");
		}
	}

	CHECK_ERR(!(tilesRows = json_object_get_array(root, "tiles")), ERR_INV_VAL);

	rows = json_array_get_count(tilesRows);
	tileHeight = (levelHeight - ((levelHeight * (up + down)) / 100)) / rows;
	GLfloat columnsWidth[rows];

	if (left) {
		xoffset = (levelWidth * left) / 100;
	}
	if (up) {
		yoffset = (levelHeight * up) / 100;
	}

	for (GLuint y = 0 ; y < rows ; y++) {
		CHECK_ERR(!(tilesColumn = json_array_get_array(tilesRows, y)), ERR_INV_VAL);
		columns = json_array_get_count(tilesColumn);
		if ((left + right) > 0) {
			columnsWidth[y] = (GLfloat)(levelWidth - ((levelWidth * (left + right)) / 100)) / (GLfloat)columns;
		} else {
			columnsWidth[y] = (GLfloat)levelWidth/(GLfloat)columns;
		}
		for (GLuint x = 0 ; x < columns ; x++) {
			GLuint isSolid = 0, visible = 0;
			GLfloat red = 0.0f, green = 0.0f, blue = 0.0f;
			JSON_Object* color = nullptr;
			JSON_Object* tile = json_array_get_object(tilesColumn, x);
			CHECK_ERR(!tile, ERR_INV_PAR);

			isSolid = json_object_get_boolean(tile, "solid");
			visible = json_object_get_boolean(tile, "visible");

			color = json_object_get_object(tile, "color");
			if (color) {
				red = json_object_get_number(color, "red");
				green = json_object_get_number(color, "green");
				blue = json_object_get_number(color, "blue");
			}

			glm::vec2 pos((columnsWidth[y] * x) + xoffset, (tileHeight * y) + yoffset);
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
