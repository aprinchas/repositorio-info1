#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <curl/curl.h>

//Repo: https://github.com/aprinchas/repositorio-info1

struct memory {
  char *response;
  size_t size;
};

static size_t cb(char *data, size_t size, size_t nmemb, void *clientp) //Funcion callback
{
  size_t realsize = nmemb;
  struct memory *mem = clientp;

  char *ptr = realloc(mem->response, mem->size + realsize + 1);
  if(!ptr)
    return 0;  

  mem->response = ptr;
  memcpy(&(mem->response[mem->size]), data, realsize);
  mem->size += realsize;
  mem->response[mem->size] = 0;

  return realsize;
}

/*estas funciones extraen su respectivo elemento del chunk.response y lo guardan en la variable correspondiente
   devuelven 0 si no se encontro nada y 1 si hay algun dato, por lo que se pueden usar para saber si hay datos nuevos*/

int extraer_update_id(const char *json, int *update_id) { 
  const char *ptr = strstr(json, "\"update_id\"");  //strstr busca en una cadena(json) a un elemento(uptade_id) y devuelve el puntero a esa posicion
  if (ptr == NULL) 
  return 0;

  if(sscanf(ptr, "\"update_id\": %d", update_id) == 1) //con ese puntero sscanf sabe donde buscar a elemento uptade_id
    return 1;                                           // esta misma logica se aplica a todas las funciones extraer_...

  else
    return 0;
}

int extraer_chat_id(const char *json, long long *chat_id) {
    const char *ptr = strstr(json, "\"chat\"");
    if (!ptr) return 0;

    if(sscanf(ptr, "\"chat\": {\"id\": %lld", chat_id) == 1)
      return 1;

    else
      return 0;
}

int extraer_nombre(const char *json, char *nombre) {
    const char *ptr = strstr(json, "\"first_name\"");
    if (!ptr) return 0;

    if(sscanf(ptr, "\"first_name\":\"%255[^\"]", nombre) == 1)
      return 1;

    else
      return 0;
}

int extraer_texto(const char *json, char *text) {
    const char *ptr = strstr(json, "\"text\"");
    if (!ptr) return 0;

    if(sscanf(ptr, "\"text\": \"%255[^\"]", text) == 1)
      return 1;

    else
      return 0;
}

int extraer_fecha(const char *json, long *fecha) {
    const char *ptr = strstr(json, "\"date\"");
    if (!ptr) return 0;

    if(sscanf(ptr, "\"date\": %ld", fecha) == 1)
      return 1;

    else
      return 0;
    
}

int obtener_ultimo_update_id(const char *json) {         //Solo esta en caso de que se por alguna razon algun mensaje quedo "en cola"
    int id = 0;
    int max_id = 0;
    const char *p = json;

    while ((p = strstr(p, "\"update_id\"")) != NULL) {
        if (sscanf(p, "\"update_id\": %d", &id) == 1) {
            if (id > max_id)
                max_id = id;
        }
        p++;
    }

    return max_id;
}

int main(int argc, char *argv[])
{
  int update_id = 0;
  long long chat_id = 0;
  char token[64] = {0};
  char text[256] = {0};
  char respuesta[256] = {0};
  char nombre[64] = {0};
  long fecha = 0;
  int last_update_id = 0;
  struct memory chunk = {0};
  char api_url[512];

  if (argc < 2) {           //verifico que se haya ingresado un argumento
    printf("No se paso el token como argumento");
    return 1;
  }

  FILE *f = fopen(argv[1], "r");   //manejo del archivo del token

  if (f == NULL) {
    printf("No se pudo abrir el archivo del token.\n");
    return 1;
  }

  fscanf(f, "%s", token);
  fclose(f);

  CURL *curl = curl_easy_init();
  if(!curl) return 1;

  sprintf(api_url, "https://api.telegram.org/bot8279223639:%s/getUpdates?offset=%d", token, last_update_id + 1); //ver se last uptade no sirve, se hace despues

  curl_easy_setopt(curl, CURLOPT_URL, api_url);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
  CURLcode res = curl_easy_perform(curl);

  /* como el bot puede llegar a comienzar a leer desde el primer mensaje enviado en todo el historial
   es necesario saber cual es el ultimo mensaje antes de entrar al while */
  last_update_id = obtener_ultimo_update_id(chunk.response);

  //archivo para guardar la conversacion
  FILE *conv = fopen("conversacion.txt","w");

  printf("Empezando desde update_id = %d\n", last_update_id);

  if(res != CURLE_OK) {
    printf("Error en cURL: %d\n", res);
  } 
  else {

    while(1) {

      chunk.response = NULL;
      chunk.size = 0;

      sprintf(api_url, "https://api.telegram.org/bot%s/getUpdates?offset=%d", token, last_update_id + 1);

      curl_easy_setopt(curl, CURLOPT_URL, api_url);
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

      CURLcode res = curl_easy_perform(curl);

      if (res != CURLE_OK) {
        printf("Error en cURL: %s\n", curl_easy_strerror(res));
        } 
      else {
        // Procesar chunk.response
        if (extraer_update_id(chunk.response, &update_id)) { /*uso extraer_update_id para ver si hay algun mensaje nuevo, podria usar
                                                              cualquiera de las funciones extraer_...*/
          extraer_chat_id(chunk.response, &chat_id);
          extraer_texto(chunk.response, text);
          extraer_nombre(chunk.response, nombre);
          extraer_fecha(chunk.response, &fecha);

          fprintf(conv, "Fecha:%d\nUsuario:%s\nMensaje:%s\n\n", fecha, nombre, text);  //guardo la conversacion
          printf("Update: %d\n", update_id);
          printf("Chat: %lld\n", chat_id);
          printf("Texto: %s\n", text);

          last_update_id = update_id;

          if(strstr(text, "hola") != NULL){ //si strstr devuelve un puntero significa que encontro el elemento "hola"
            sprintf(respuesta, "https://api.telegram.org/bot%s/sendMessage?chat_id=%lld&text=Hola%%20%s!", token, chat_id, nombre); //respuesta a hola

            free(chunk.response);       // se puede liberar la memoria porque al enviar el bot un mensaje se activa el callback tambien
            chunk.response = NULL;
            chunk.size = 0;
            curl_easy_setopt(curl, CURLOPT_URL, respuesta);
            curl_easy_perform(curl);

            extraer_texto(chunk.response, text);
            extraer_nombre(chunk.response, nombre);
            extraer_fecha(chunk.response, &fecha);

            fprintf(conv, "Fecha:%d\nUsuario:%s\nMensaje:%s\n\n", fecha, nombre, text);
          }

          if(strstr(text, "chau") != NULL){
            sprintf(respuesta, "https://api.telegram.org/bot%s/sendMessage?chat_id=%lld&text=Chau%%20%s", token, chat_id, nombre); //respuesta a chau

            free(chunk.response);     
            chunk.response = NULL;
            chunk.size = 0;
            curl_easy_setopt(curl, CURLOPT_URL, respuesta);
            curl_easy_perform(curl);

            extraer_texto(chunk.response, text);
            extraer_nombre(chunk.response, nombre);
            extraer_fecha(chunk.response, &fecha);

            fprintf(conv, "Fecha:%d\nUsuario:%s\nMensaje:%s\n\n", fecha, nombre, text);
          }
        }
      }   
            
    free(chunk.response);
    sleep(2); // espera 2 segundos
    }
  }
    
  fclose(conv);
  curl_easy_cleanup(curl);
  return 0;
}