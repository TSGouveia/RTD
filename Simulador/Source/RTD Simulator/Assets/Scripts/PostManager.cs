using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Net;
using System.Text;
using System.Threading;
using System.Web;
using Unity.VisualScripting.FullSerializer;
using UnityEditor.PackageManager.Requests;
using UnityEngine;

public class PostManager : MonoBehaviour
{
    private HttpListenerContext context;
    private HttpListener listener;
    private Thread listenerThread;

    private SkillManager skillManager;
    void Start()
    {
        StartServer();
        skillManager = FindObjectOfType<SkillManager>();
    }
    void StartServer()
    {
        // Replace this with the port you want to listen on
        string url = "http://localhost:80/transporte/";

        listener = new HttpListener();
        listener.Prefixes.Add(url);

        listener.Start();

        Debug.Log("Server started. Listening for incoming requests...");

        // Start listening for incoming requests on a separate thread
        listenerThread = new Thread(new ThreadStart(ListenForRequests));
        listenerThread.Start();
    }
    void OnDisable()
    {
        // Stop the listener when the script is disabled or the game is closed
        if (listener != null && listener.IsListening)
        {
            listener.Stop();
            Debug.Log("Server stopped.");
        }

        // Stop the listener thread
        if (listenerThread != null && listenerThread.IsAlive)
        {
            listenerThread.Abort();
        }
    }

    void ListenForRequests()
    {
        while (listener.IsListening)
        {
            // Wait for a request to come in
            context = listener.GetContext();


            // Process the request on the main thread
            HandleRequest(context);
        }
    }

    void EnviaResposta(string str)
    {
        string responseString = str;
        byte[] responsebytes = Encoding.UTF8.GetBytes(responseString);
        context.Response.OutputStream.Write(responsebytes, 0, responsebytes.Length);
        context.Response.Close();
    }

    void HandleRequest(HttpListenerContext context)
    {
        if (context.Request.HttpMethod == "POST")
        {
            // Read the request body
            using (Stream body = context.Request.InputStream)
            {
                using (StreamReader reader = new StreamReader(body, context.Request.ContentEncoding))
                {
                    string queryString = context.Request.Url.Query;
                    string skill = HttpUtility.ParseQueryString(queryString).Get("skill");
                    
                    Debug.Log("Received POST: " + skill);

                    skillManager.StartSkill(skill);

                    EnviaResposta("Saga");
                }
            }
        }
        else
        {
            // Handle other HTTP methods or provide an error response
            context.Response.StatusCode = (int)HttpStatusCode.MethodNotAllowed;
            context.Response.Close();
        }
    }
}
