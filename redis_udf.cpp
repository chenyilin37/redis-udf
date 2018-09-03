#include <mysql.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include "redis_client.h"
using namespace std;

#define SUCCESS "SUCCESS"
#define RESULT(x) setResult(result,length,x)
#define STRING_RESULT(x) setStringResult(result,length,x)

extern "C" void setResult(char* result,unsigned long * length,const char *resultValue)
{
	if(resultValue){
		int len = strlen(resultValue);
		memcpy(result,resultValue,len);
		*length = len;
	}
	else{
		const char* c_ret = "null";
		int len = strlen(c_ret);
		memcpy(result,c_ret,len);
		*length = len;
	}
}

extern "C" void setStringResult(char* result,unsigned long * length,string_type resultValue)
{
	const char *c_retValue = resultValue.c_str();
	setResult(result,length,c_retValue);
}

extern "C" char *redis_hset(UDF_INIT *initid, UDF_ARGS *args, char *result, unsigned long *length, char *is_null, char *error){
	memset(result,0,sizeof(result));
	if(!(args->args && args->args[0] && args->args[1] && args->args[2])){
      *is_null = 1;
      RESULT(NULL);
      return result;
   }
   try{
   	RedisClient *p_client = init_client_if_isnull();
   	p_client->hset(args->args[0], args->args[1], args->args[2]);
   	RESULT(SUCCESS);
   	return result;
 	}
 	catch(redis_error & e){
 		string errMsg(e);
 		STRING_RESULT(errMsg);
 		return result;
 	}
}


extern "C" my_bool redis_hset_init(UDF_INIT *initid, UDF_ARGS *args, char *message)
{
    if (3 != args->arg_count  || args->arg_type[0] != STRING_RESULT  || args->arg_type[1] != STRING_RESULT  || args->arg_type[2] != STRING_RESULT){ // hset(key, field, value) 需要三个参数
        strncpy(message, "please input 2 args and must be string, such as: hset('key', 'feild', 'value');", MYSQL_ERRMSG_SIZE);
        return -1;
    }
    args->arg_type[0] = STRING_RESULT;
    args->arg_type[1] = STRING_RESULT;
    args->arg_type[2] = STRING_RESULT;

    initid->ptr       = NULL;
    return 0;
}

extern "C" char *redis_hget(UDF_INIT *initid, UDF_ARGS *args, char *result, unsigned long *length, char *is_null, char *error){
	memset(result,0,sizeof(result));
	if(!(args->args && args->args[0] && args->args[1])){
      *is_null = 1;
      RESULT(NULL);
      return result;
   }
   try{
   	RedisClient *p_client = init_client_if_isnull();
   	string_type ret = p_client->hget(args->args[0],args->args[1]);
   	STRING_RESULT(ret);
  	return result;
 	}
 	catch(redis_error & e){
 		string errMsg(e);
 		STRING_RESULT(errMsg);
 		return result;
 	}
}


extern "C" my_bool redis_hget_init(UDF_INIT *initid, UDF_ARGS *args, char *message)
{
    if (2 != args->arg_count  || args->arg_type[0] != STRING_RESULT  || args->arg_type[1] != STRING_RESULT ){ // hset(key, field, value) 需要三个参数
        strncpy(message, "please input 3 args and must be string, such as: hget('key', 'feild');", MYSQL_ERRMSG_SIZE);
        return -1;
    }
    args->arg_type[0] = STRING_RESULT;
    args->arg_type[1] = STRING_RESULT;

    initid->ptr       = NULL;
    return 0;
}

extern "C" char *redis_del(UDF_INIT *initid, UDF_ARGS *args, char *result, unsigned long *length, char *is_null, char *error){
	memset(result,0,sizeof(result));
	if(!(args->args && args->args[0])){
      *is_null = 1;
      RESULT(NULL);
      return result;
   }
   try{
   	RedisClient *p_client = init_client_if_isnull();
   	p_client->del(args->args[0]);
   	RESULT(SUCCESS);
  	return result;
 	}
 	catch(redis_error & e){
 		string errMsg(e);
 		STRING_RESULT(errMsg);
 		return result;
 	}
}


extern "C" my_bool redis_del_init(UDF_INIT *initid, UDF_ARGS *args, char *message)
{
    if (1 != args->arg_count  || args->arg_type[0] != STRING_RESULT){ // hset(key, field, value) 需要三个参数
        strncpy(message, "please input 1 args and must be string, such as: del('key');", MYSQL_ERRMSG_SIZE);
        return -1;
    }
    args->arg_type[0] = STRING_RESULT;

    initid->ptr       = NULL;
    return 0;
}


extern "C" char *redis_set(UDF_INIT *initid, UDF_ARGS *args, char *result, unsigned long *length, char *is_null, char *error){
	memset(result,0,sizeof(result));
	if(!(args->args && args->args[0] && args->args[1])){
      *is_null = 1;
      RESULT(NULL);
      return result;
   }
   try{
   	RedisClient *p_client = init_client_if_isnull();
   	p_client->set(args->args[0],args->args[1]);
   	RESULT(SUCCESS);
  	return result;
 	}
 	catch(redis_error & e){
 		string errMsg(e);
 		STRING_RESULT(errMsg);
 		return result;
 	}
}


extern "C" my_bool redis_set_init(UDF_INIT *initid, UDF_ARGS *args, char *message)
{
    if (2 != args->arg_count  || args->arg_type[0] != STRING_RESULT || args->arg_type[1] != STRING_RESULT){ // hset(key, field, value) 需要三个参数
        strncpy(message, "please input 2 args and must be string, such as: set('key','value');", MYSQL_ERRMSG_SIZE);
        return -1;
    }
    args->arg_type[0] = STRING_RESULT;
		args->arg_type[1] = STRING_RESULT;
    initid->ptr       = NULL;
    return 0;
}


extern "C" char *redis_get(UDF_INIT *initid, UDF_ARGS *args, char *result, unsigned long *length, char *is_null, char *error){
	memset(result,0,sizeof(result));
	if(!(args->args && args->args[0])){
      *is_null = 1;
      RESULT(NULL);
      return result;
   }
   try{
   	RedisClient *p_client = init_client_if_isnull();
   	string_type ret = p_client->get(args->args[0]);
   	STRING_RESULT(ret);
  	return result;
 	}
 	catch(redis_error & e){
 		string errMsg(e);
 		STRING_RESULT(errMsg);
 		return result;
 	}
}


extern "C" my_bool redis_get_init(UDF_INIT *initid, UDF_ARGS *args, char *message)
{
    if (1 != args->arg_count  || args->arg_type[0] != STRING_RESULT ){ // hset(key, field, value) 需要三个参数
        strncpy(message, "please input 2 args and must be string, such as: get('key');", MYSQL_ERRMSG_SIZE);
        return -1;
    }
    args->arg_type[0] = STRING_RESULT;
    initid->ptr       = NULL;
    return 0;
}

extern "C" char *redis_hmget(UDF_INIT *initid, UDF_ARGS *args, char *result, unsigned long *length, char *is_null, char *error){
	memset(result,0,sizeof(result));
   try{
   	string_vector fields = string_vector();
   	string_vector out = string_vector();
   	for(int i = 1;i < args->arg_count;i++)
   	{
   		string_type field(args->args[i]);
   		fields.push_back(field);
   	}
   	
   	RedisClient *p_client = init_client_if_isnull();
   	p_client->hmget(args->args[0],fields,out);
   	if(out.size() > 0)
 		{
 			string ret("");
 			int size = out.size();
 			for(int i = 0;i < size;i++)
 			{
 				ret += out[i];
 				if(i < size -1)
 					ret += ",";
 			}
 			STRING_RESULT(ret);
 		}
 		else{
 			RESULT(NULL);
 		}
  	return result;
 	}
 	catch(redis_error & e){
 		string errMsg(e);
 		STRING_RESULT(errMsg);
 		return result;
 	}
}


extern "C" my_bool redis_hmget_init(UDF_INIT *initid, UDF_ARGS *args, char *message)
{
    if (args->arg_count < 2 ){ // hset(key, field, value) 需要三个参数
        strncpy(message, "please input 2 or more args and must be string, such as: hmget('key',id1,id2...);", MYSQL_ERRMSG_SIZE);
        return -1;
    }
    for(int i = 0;i < args->arg_count;i++)
    {
    	args->arg_type[i] = STRING_RESULT;
    }
    initid->ptr       = NULL;
    return 0;
}

extern "C" char *redis_hdel(UDF_INIT *initid, UDF_ARGS *args, char *result, unsigned long *length, char *is_null, char *error){
	memset(result,0,sizeof(result));
   try{
   	string_vector fields = string_vector();
   	for(int i = 1;i < args->arg_count;i++)
   	{
   		string element(args->args[i]);
   		fields.push_back(element);
   	}
   	
   	RedisClient *p_client = init_client_if_isnull();
   	p_client->hdel(args->args[0],fields);
   	RESULT(SUCCESS);
  	return result;
 	}
 	catch(redis_error & e){
 		string errMsg(e);
 		STRING_RESULT(errMsg);
 		return result;
 	}
}

extern "C" my_bool redis_hdel_init(UDF_INIT *initid, UDF_ARGS *args, char *message)
{
    if (args->arg_count < 2 ){ // hset(key, field, value) 需要三个参数
        strncpy(message, "please input 2 or more args and must be string, such as: hdel('key',id1,id2...);", MYSQL_ERRMSG_SIZE);
        return -1;
    }
    for(int i = 0;i < args->arg_count;i++)
    {
    	args->arg_type[i] = STRING_RESULT;
    }
    initid->ptr       = NULL;
    return 0;
}

extern "C" char *redis_sadd(UDF_INIT *initid, UDF_ARGS *args, char *result, unsigned long *length, char *is_null, char *error){
	memset(result,0,sizeof(result));
   try{
   	string_vector fields = string_vector();
   	for(int i = 1;i < args->arg_count;i++)
   	{
   		string element(args->args[i]);
   		fields.push_back(element);
   	}
   	
   	RedisClient *p_client = init_client_if_isnull();
   	p_client->sadd(args->args[0],fields);
   	RESULT(SUCCESS);
  	return result;
 	}
 	catch(redis_error & e){
 		string errMsg(e);
 		STRING_RESULT(errMsg);
 		return result;
 	}
}

extern "C" my_bool redis_sadd_init(UDF_INIT *initid, UDF_ARGS *args, char *message)
{
    if (args->arg_count < 2 ){ // hset(key, field, value) 需要三个参数
        strncpy(message, "please input 2 or more args and must be string, such as: sadd('key',id1,id2...);", MYSQL_ERRMSG_SIZE);
        return -1;
    }
    for(int i = 0;i < args->arg_count;i++)
    {
    	args->arg_type[i] = STRING_RESULT;
    }
    initid->ptr       = NULL;
    return 0;
}

extern "C" char *redis_srem(UDF_INIT *initid, UDF_ARGS *args, char *result, unsigned long *length, char *is_null, char *error){
	memset(result,0,sizeof(result));
   try{
   	string_vector fields = string_vector();
   	for(int i = 1;i < args->arg_count;i++)
   	{
   		string element(args->args[i]);
   		fields.push_back(element);
   	}
   	
   	RedisClient *p_client = init_client_if_isnull();
   	p_client->srem(args->args[0],fields);
   	RESULT(SUCCESS);
  	return result;
 	}
 	catch(redis_error & e){
 		string errMsg(e);
 		STRING_RESULT(errMsg);
 		return result;
 	}
}

extern "C" my_bool redis_srem_init(UDF_INIT *initid, UDF_ARGS *args, char *message)
{
    if (args->arg_count < 2 ){ // hset(key, field, value) 需要三个参数
        strncpy(message, "please input 2 or more args and must be string, such as: srem('key',id1,id2...);", MYSQL_ERRMSG_SIZE);
        return -1;
    }
    for(int i = 0;i < args->arg_count;i++)
    {
    	args->arg_type[i] = STRING_RESULT;
    }
    initid->ptr       = NULL;
    return 0;
}


extern "C" char *redis_zadd(UDF_INIT *initid, UDF_ARGS *args, char *result, unsigned long *length, char *is_null, char *error){
	memset(result,0,sizeof(result));
   try{
   	string_vector fields = string_vector();
   	string_vector values = string_vector();
   	for(int i = 1;i < args->arg_count;i++)
   	{
   		string element(args->args[i]);
   		if(i % 2 == 0){
   			values.push_back(element);
   		}
   		else{
   			fields.push_back(element);
   		}
   	}
   	
   	RedisClient *p_client = init_client_if_isnull();
   	p_client->zadd(args->args[0],fields,values);
   	RESULT(SUCCESS);
  	return result;
 	}
 	catch(redis_error & e){
 		string errMsg(e);
 		STRING_RESULT(errMsg);
 		return result;
 	}
}


extern "C" my_bool redis_zadd_init(UDF_INIT *initid, UDF_ARGS *args, char *message)
{
    if (args->arg_count < 3 ){
        strncpy(message, "please input 3 or more args and must be string, such as: zadd('key',id1,value1,...);", MYSQL_ERRMSG_SIZE);
        return -1;
    }
    for(int i = 0;i < args->arg_count;i++)
    {
    	args->arg_type[i] = STRING_RESULT;
    }
    initid->ptr       = NULL;
    return 0;
}

extern "C" char *redis_zrem(UDF_INIT *initid, UDF_ARGS *args, char *result, unsigned long *length, char *is_null, char *error){
	memset(result,0,sizeof(result));
   try{
   	string_vector fields = string_vector();
   	for(int i = 1;i < args->arg_count;i++)
   	{
   		string element(args->args[i]);
   		fields.push_back(element);
   	}
   	
   	RedisClient *p_client = init_client_if_isnull();
   	p_client->zrem(args->args[0],fields);
   	RESULT(SUCCESS);
  	return result;
 	}
 	catch(redis_error & e){
 		string errMsg(e);
 		STRING_RESULT(errMsg);
 		return result;
 	}
}


extern "C" my_bool redis_zrem_init(UDF_INIT *initid, UDF_ARGS *args, char *message)
{
    if (args->arg_count < 2 ){ // hset(key, field, value) 需要三个参数
        strncpy(message, "please input 2 or more args and must be string, such as: zrem('key',id1,id2...);", MYSQL_ERRMSG_SIZE);
        return -1;
    }
    for(int i = 0;i < args->arg_count;i++)
    {
    	args->arg_type[i] = STRING_RESULT;
    }
    initid->ptr       = NULL;
    return 0;
}

extern "C" char *redis_hmset(UDF_INIT *initid, UDF_ARGS *args, char *result, unsigned long *length, char *is_null, char *error){
	memset(result,0,sizeof(result));
   try{
   	string_vector fields = string_vector();
   	string_vector values = string_vector();
   	for(int i = 1;i < args->arg_count;i++)
   	{
   		string element(args->args[i]);
   		if(i % 2 == 0){
   			values.push_back(element);
   		}
   		else{
   			fields.push_back(element);
   		}
   	}
   	
   	RedisClient *p_client = init_client_if_isnull();
   	p_client->hmset(args->args[0],fields,values);
   	RESULT(SUCCESS);
  	return result;
 	}
 	catch(redis_error & e){
 		string errMsg(e);
 		STRING_RESULT(errMsg);
 		return result;
 	}
}


extern "C" my_bool redis_hmset_init(UDF_INIT *initid, UDF_ARGS *args, char *message)
{
    if (args->arg_count < 3 ){
        strncpy(message, "please input 3 or more args and must be string, such as: hmset('key',id1,value1,...);", MYSQL_ERRMSG_SIZE);
        return -1;
    }
    for(int i = 0;i < args->arg_count;i++)
    {
    	args->arg_type[i] = STRING_RESULT;
    }
    initid->ptr       = NULL;
    return 0;
}


extern "C" char *redis_getset(UDF_INIT *initid, UDF_ARGS *args, char *result, unsigned long *length, char *is_null, char *error){
	memset(result,0,sizeof(result));
	if(!(args->args && args->args[0] && args->args[1])){
      *is_null = 1;
      RESULT(NULL);
      return result;
   }
   try{
   	RedisClient *p_client = init_client_if_isnull();
   	string_type ret = p_client->getset(args->args[0],args->args[1]);
   	STRING_RESULT(ret);
   	return result;
 	} catch(redis_error & e){
 		string errMsg(e);
 		STRING_RESULT(errMsg);
 		return result;
 	}
  
}


extern "C" my_bool redis_getset_init(UDF_INIT *initid, UDF_ARGS *args, char *message)
{
    if (2 != args->arg_count  || args->arg_type[0] != STRING_RESULT  || args->arg_type[1] != STRING_RESULT){ // hset(key, field, value) 需要三个参数
        strncpy(message, "please input 2 args and must be string, such as: getset('key', 'value');", MYSQL_ERRMSG_SIZE);
        return -1;
    }
    args->arg_type[0] = STRING_RESULT;
    args->arg_type[1] = STRING_RESULT;

    initid->ptr       = NULL;
    return 0;
}

extern "C" long long test_add(UDF_INIT *initid, UDF_ARGS *args, char *result, unsigned long *length, char *is_null, char *error)
{
    int a = *((long long *)args->args[0]);
    int b = *((long long *)args->args[1]);
    return a + b;
}

extern "C" my_bool test_add_init(UDF_INIT *initid, UDF_ARGS *args, char *message)
{
    return 0;
}


extern "C" char *show_envs(UDF_INIT *initid, UDF_ARGS *args, char *result, unsigned long *length, char *is_null, char *error){
	memset(result,0,sizeof(result));
   try{
   	const char* ret = getenv(args->args[0]);
   	if(!ret)
   		ret = "NULL";
   		
   	STRING_RESULT(ret);
   	
   	return result;
 	} catch(redis_error & e){
 		string errMsg(e);
 		STRING_RESULT(errMsg);
 		return result;
 	}
  
}

extern "C" my_bool show_envs_init(UDF_INIT *initid, UDF_ARGS *args, char *message)
{
    return 0;
}
