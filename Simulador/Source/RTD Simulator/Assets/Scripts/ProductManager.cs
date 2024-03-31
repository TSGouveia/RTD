using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ProductManager : MonoBehaviour
{
    [SerializeField]
    private Transform productSpawnLocation;

    [SerializeField]
    private GameObject productPrefab;

    private Queue<GameObject> productQueue = new Queue<GameObject>();
    public void SpawnProduct()
    {
        productQueue.Enqueue(Instantiate(productPrefab, productSpawnLocation));
    }
    public void DestroyProduct()
    {
        GameObject toDestroy = productQueue.Dequeue();

        if (toDestroy == null)
            return;

        Destroy(toDestroy);
    }
}
