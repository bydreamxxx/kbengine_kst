/*
This source file is part of KBEngine
For the latest info, see http://www.kbengine.org/

Copyright (c) 2008-2018 KBEngine.

KBEngine is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

KBEngine is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.
 
You should have received a copy of the GNU Lesser General Public License
along with KBEngine.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef KBE_DATA_TYPES_H
#define KBE_DATA_TYPES_H

#include "common/common.h"
#if KBE_PLATFORM == PLATFORM_WIN32
#pragma warning (disable : 4910)
#pragma warning (disable : 4251)
#endif
// common include	
#include "datatype.h"
#include "xml/xml.h"	
#include "common/smartpointer.h"
#include "common/utils.h"

namespace KBEngine{
typedef SmartPointer<DataType> DataTypePtr;

class DataTypes
{
public:	
	typedef std::map<std::string, DataTypePtr> DATATYPE_MAP;
	typedef std::map<DATATYPE_UID, DataType*> UID_DATATYPE_MAP;
	typedef std::vector< std::string > DATATYPE_ORDERS;

	DataTypes();
	virtual ~DataTypes();	

	static bool initialize(std::string file);
	static void finalise(void);

	static bool addDataType(std::string name, DataType* dataType);
	static bool addDataType(DATATYPE_UID uid, DataType* dataType);
	static void delDataType(std::string name);

	static DataType* getDataType(std::string name);
	static DataType* getDataType(const char* name);
	static DataType* getDataType(DATATYPE_UID uid);

	static bool validTypeName(const std::string& typeName);

	static bool loadTypes(std::string& file);
	static bool loadTypes(std::shared_ptr<XML> xml);

	static const DATATYPE_MAP& dataTypes() { return dataTypes_; }
	static const UID_DATATYPE_MAP& uid_dataTypes() { return uid_dataTypes_; }
	static const DATATYPE_ORDERS& dataTypesOrders() { return dataTypesOrders_; }

	static std::vector< std::string > getBaseTypeNames();

protected:
	static DATATYPE_MAP dataTypes_;
	static DATATYPE_MAP dataTypesLowerName_;
	static UID_DATATYPE_MAP uid_dataTypes_;

	// 类型定义的先后顺序，用于代码生成， c++等语言需要先后顺序依赖
	static DATATYPE_ORDERS dataTypesOrders_;
};

}
#endif // KBE_DATA_TYPES_H
