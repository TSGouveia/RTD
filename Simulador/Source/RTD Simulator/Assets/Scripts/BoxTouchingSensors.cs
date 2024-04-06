using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BoxTouchingSensors : MonoBehaviour
{
    private const string SENSORTAG = "Sensor";
    Sensor sensorImTouching;
    private void OnTriggerEnter(Collider other)
    {
        if (other.gameObject.CompareTag(SENSORTAG))
        {
            sensorImTouching = other.GetComponent<Sensor>();
        }
    }
    private void OnTriggerExit(Collider other)
    {
        if (other.gameObject.CompareTag(SENSORTAG))
        {
            sensorImTouching = null;
        }
    }
    private void OnDestroy()
    {
        if (sensorImTouching != null)
        {
            sensorImTouching.ObjectIsBeingDestroyed();
        }
    }
}
