# Generated by Django 3.1.4 on 2021-01-10 15:42

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('algorithms', '0016_algorithm_se'),
    ]

    operations = [
        migrations.AlterField(
            model_name='algorithm',
            name='SE',
            field=models.FloatField(default=0),
        ),
    ]
